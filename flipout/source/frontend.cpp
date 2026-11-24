/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <algorithm>
#include <msp/gl/matrix.h>
#include <msp/gl/projection.h>
#include <msp/gltk/button.h>
#include <msp/gltk/slider.h>
#include <msp/gltk/toggle.h>
#include <msp/strings/formatter.h>
#include <msp/strings/regex.h>
#include <msp/time/datetime.h>
#include <msp/time/units.h>
#include <msp/time/utils.h>
#include "editor.h"
#include "flipout.h"
#include "frontend.h"
#include "game.h"
#include "level.h"
#include "music.h"
#include "resources.h"
#include "series.h"

using namespace std;
using namespace Msp;

namespace {

template<typename T>
void set_from_toggle(bool v, T *s)
{
	*s=v;
}

template<typename T>
void set_from_dropdown(unsigned i, const string &, T *s)
{
	*s=i;
}

template<typename T>
void set_from_slider(double v, T *s)
{
	*s=static_cast<T>(v);
}

struct Size
{
	unsigned w, h;

	Size(unsigned w_, unsigned h_): w(w_), h(h_) { }
	bool operator<(const Size &s) const { return w<s.w || (w==s.w && h<s.h); }
};

}

FrontEnd::FrontEnd(FlipOut &fo):
	State(fo),
	selected_level(0)
{
	const Resources &res=flipout.get_resources();

	ui=res.get<GLtk::UserInterface>("frontend");

	root=&ui->get_root();
	
	GLtk::Panel *pnl_mainmenu;
	ui->get_widget("pnl_mainmenu", pnl_mainmenu);
	panels.push_back(pnl_mainmenu);

	list<string> names=res.get_names<Level>();
	for(list<string>::const_iterator i=names.begin(); i!=names.end(); ++i)
	{
		LevelInfo info;
		info.level=res.get<Level>(*i);
		info.name=*i;
		info.label=*i;
		if(!info.level->get_title().empty())
			info.label=info.level->get_title();
		levels.push_back(info);
	}
	sort(levels.begin(), levels.end());

	names=res.get_names<Series>();
	series.assign(names.begin(), names.end());

	Connector conn(*this);
	conn.connect(ui->get_logic());

	flipout.get_highscores().signal_scores_changed.connect(sigc::mem_fun(this, &FrontEnd::highscores_changed));
}

void FrontEnd::enable()
{
	root->set_visible(true);
}

void FrontEnd::disable()
{
	root->set_visible(false);
}

void FrontEnd::render() const
{
	const GLtk::Geometry &root_geom=root->get_geometry();

	GL::matrix_mode(GL::PROJECTION);
	GL::load_identity();
	GL::ortho_bottomleft(root_geom.w, root_geom.h);
	GL::matrix_mode(GL::MODELVIEW);
	GL::load_identity();

	root->render();
}

void FrontEnd::quit()
{
	finished=true;
}

void FrontEnd::start_series(const string &name)
{
	flipout.push_state(new Game(flipout, *flipout.get_resources().get<Series>(name), "S"+name));
}

void FrontEnd::start_selected_level()
{
	if(selected_level)
		flipout.push_state(new Game(flipout, *selected_level->level, "L"+selected_level->name));
}

void FrontEnd::show_panel(Msp::GLtk::Panel *panel)
{
	if(!panels.empty())
		panels.back()->set_visible(false);
	panels.push_back(panel);
	panel->set_visible(true);
}

void FrontEnd::close_panel()
{
	if(panels.empty())
		return;

	panels.back()->set_visible(false);
	panels.pop_back();
	if(!panels.empty())
		panels.back()->set_visible(true);
}

void FrontEnd::capture_control(ControlInfo *cinfo)
{
	cinfo->ctrl->capture(flipout.get_hub());

	GLtk::Panel *pnl;
	ui->get_widget("pnl_capture_control", pnl);
	show_panel(pnl);

	GLtk::Label *lbl;
	ui->get_widget("lbl_control_name", lbl);
	lbl->set_text(cinfo->name);
}

void FrontEnd::control_captured(ControlInfo *cinfo)
{
	close_panel();

	const Input::ControlSource &src=cinfo->ctrl->get_source();
	cinfo->btn->set_text(src.str());

	flipout.get_settings().controls[cinfo->name]=(src.type<<24)|src.index;
}

void FrontEnd::level_selected(unsigned index, const string &)
{
	selected_level=&levels[index];
}

void FrontEnd::resolution_selected(unsigned, const string &str)
{
	RegMatch m=Regex("([0-9]+)x([0-9]+)").match(str);
	unsigned width=lexical_cast<unsigned>(m[1].str);
	unsigned height=lexical_cast<unsigned>(m[2].str);

	flipout.get_settings().width=width;
	flipout.get_settings().height=height;

	Graphics::WindowOptions wopt=flipout.get_window().get_options();
	wopt.width=width;
	wopt.height=height;
	flipout.get_window().reconfigure(wopt);
}

void FrontEnd::set_fullscreen(bool fs)
{
	Graphics::WindowOptions wopt=flipout.get_window().get_options();
	wopt.fullscreen=fs;
	flipout.get_window().reconfigure(wopt);
}

void FrontEnd::set_vertex_buffers(bool vb)
{
	list<GL::Mesh *> meshes=flipout.get_resources().get_list<GL::Mesh>();
	for(list<GL::Mesh *>::iterator i=meshes.begin(); i!=meshes.end(); ++i)
		(*i)->use_vertex_buffer(vb);
}

void FrontEnd::set_music_enabled(bool e)
{
	if(e)
		flipout.get_music().play();
	else
		flipout.get_music().stop();
}

void FrontEnd::set_music_volume(double vol)
{
	flipout.get_music().set_volume(static_cast<unsigned>(vol));
}

void FrontEnd::save_settings()
{
	flipout.get_settings().save();
}

void FrontEnd::highscore_level_selected(unsigned index, const string &)
{
	if(index==0 || index==series.size()+1)
		return;

	if(index<=series.size())
		selected_hs_board="S"+series[index-1];
	else
		selected_hs_board="L"+levels[index-series.size()-2].name;

	signal_highscore_board_selected.emit(selected_hs_board);
}

void FrontEnd::fill_highscore_table(const string &id, GLtk::Table *tbl)
{
	for(unsigned i=0; i<tbl->get_rows(); ++i)
		for(unsigned j=0; j<tbl->get_columns(); ++j)
			tbl->set_cell_text(i, j, string());

	try
	{
		Msp::Time::TimeStamp t=Msp::Time::now();
		const ScoreBoard &board=flipout.get_highscores().get_board(id);
		const set<ScoreBoard::Entry> &scores=board.get_scores();
		set<ScoreBoard::Entry>::const_iterator j=scores.begin();
		for(unsigned i=0; (i<tbl->get_rows() && j!=scores.end()); ++i, ++j)
		{
			tbl->set_cell_text(i, 0, lexical_cast(j->score));
			if(j->level>0)
				tbl->set_cell_text(i, 1, lexical_cast(j->level));
			else
				tbl->set_cell_text(i, 1, "-");
			tbl->set_cell_text(i, 2, j->name);
			if(t-j->time<Msp::Time::min)
				tbl->set_cell_text(i, 3, "A moment ago");
			else
				tbl->set_cell_text(i, 3, Msp::Time::DateTime(j->time).format("%Y-%m-%d"));
		}
	}
	catch(const KeyError &)
	{ }
}

void FrontEnd::highscores_changed()
{
	signal_highscore_board_selected.emit(selected_hs_board);
}

void FrontEnd::start_editor()
{
	flipout.push_state(new Editor(flipout));
}


FrontEnd::Connector::Connector(FrontEnd &fe):
	frontend(fe)
{
	add("close_panel",  &GLtk::Button::signal_clicked, &FrontEnd::close_panel);
	add("control",      &Connector::control);
	add("editor",       &GLtk::Button::signal_clicked, &FrontEnd::start_editor);
	add("highscore_levels", &Connector::highscore_levels);
	add("highscore_table",  &Connector::highscore_table);
	add("levels",       &Connector::levels);
	add("quit",         &GLtk::Button::signal_clicked, &FrontEnd::quit);
	add("resolution",   &Connector::resolution);
	add("save_settings", &GLtk::Button::signal_clicked, &FrontEnd::save_settings);
	add("setting",      &Connector::setting);
	add("show_panel",   &Connector::show_panel);
	add("start_selected_level", &GLtk::Button::signal_clicked, &FrontEnd::start_selected_level);
	add("start_series", &Connector::start_series);
}

void FrontEnd::Connector::control(GLtk::Button &btn, const string &cname)
{
	frontend.ctrl_infos.push_back(ControlInfo());

	ControlInfo &cinfo=frontend.ctrl_infos.back();
	cinfo.name=cname;
	cinfo.ctrl=&frontend.flipout.get_controls().get(cname);
	cinfo.btn=&btn;

	btn.signal_clicked.connect(sigc::bind(sigc::mem_fun(&frontend, &FrontEnd::capture_control), &cinfo));
	cinfo.ctrl->signal_capture_complete.connect(sigc::bind(sigc::mem_fun(&frontend, &FrontEnd::control_captured), &cinfo));

	btn.set_text(cinfo.ctrl->get_source().str());
}

void FrontEnd::Connector::highscore_levels(GLtk::List &lst)
{
	lst.clear();
	lst.append("--- Series ---");
	for(vector<string>::const_iterator i=frontend.series.begin(); i!=frontend.series.end(); ++i)
		lst.append(*i);

	lst.append("--- Levels ---");
	for(vector<LevelInfo>::const_iterator i=frontend.levels.begin(); i!=frontend.levels.end(); ++i)
		lst.append(i->label);

	lst.signal_item_selected.connect(sigc::mem_fun(&frontend, &FrontEnd::highscore_level_selected));
}

void FrontEnd::Connector::highscore_table(GLtk::Table &tbl)
{
	frontend.signal_highscore_board_selected.connect(sigc::bind(sigc::mem_fun(&frontend, &FrontEnd::fill_highscore_table), &tbl));
}

void FrontEnd::Connector::levels(GLtk::List &lst)
{
	for(vector<LevelInfo>::const_iterator i=frontend.levels.begin(); i!=frontend.levels.end(); ++i)
		lst.append(i->label);

	lst.signal_item_selected.connect(sigc::mem_fun(&frontend, &FrontEnd::level_selected));
}

void FrontEnd::Connector::resolution(GLtk::Dropdown &drp)
{
	const list<Graphics::VideoMode> &modes=frontend.flipout.get_display().get_modes();
	set<Size> sizes;
	for(list<Graphics::VideoMode>::const_iterator i=modes.begin(); i!=modes.end(); ++i)
		sizes.insert(Size(i->width, i->height));

	const Settings &sett=frontend.flipout.get_settings();
	for(set<Size>::const_iterator i=sizes.begin(); i!=sizes.end(); ++i)
	{
		drp.append(format("%dx%d", i->w, i->h));
		if(i->w==sett.width && i->h==sett.height)
			drp.set_selected_index(distance(sizes.begin(), i));
	}

	drp.signal_item_selected.connect(sigc::mem_fun(&frontend, &FrontEnd::resolution_selected));
}

void FrontEnd::Connector::setting(GLtk::Widget &wdg, const string &sname)
{
	if(GLtk::Toggle *tgl=dynamic_cast<GLtk::Toggle *>(&wdg))
	{
		bool *sett=frontend.flipout.get_settings().get(sname);
		tgl->set_value(*sett);
		tgl->signal_toggled.connect(sigc::bind(sigc::ptr_fun(&set_from_toggle<bool>), sett));
		if(sname=="fullscreen")
			tgl->signal_toggled.connect(sigc::mem_fun(&frontend, &FrontEnd::set_fullscreen));
		else if(sname=="vertex_buffers")
			tgl->signal_toggled.connect(sigc::mem_fun(&frontend, &FrontEnd::set_vertex_buffers));
		else if(sname=="music_enabled")
			tgl->signal_toggled.connect(sigc::mem_fun(&frontend, &FrontEnd::set_music_enabled));
	}
	else if(GLtk::Dropdown *drp=dynamic_cast<GLtk::Dropdown *>(&wdg))
	{
		unsigned *sett=frontend.flipout.get_settings().get(sname);
		drp->set_selected_index(*sett);
		drp->signal_item_selected.connect(sigc::bind(sigc::ptr_fun(&set_from_dropdown<unsigned>), sett));
	}
	else if(GLtk::Slider *sld=dynamic_cast<GLtk::Slider *>(&wdg))
	{
		unsigned *sett=frontend.flipout.get_settings().get(sname);
		sld->set_value(*sett);
		sld->signal_value_changed.connect(sigc::bind(sigc::ptr_fun(&set_from_slider<unsigned>), sett));
		if(sname=="music_volume")
			sld->signal_value_changed.connect(sigc::mem_fun(&frontend, &FrontEnd::set_music_volume));
	}
}

void FrontEnd::Connector::show_panel(GLtk::Button &btn, const string &pname)
{
	GLtk::Panel *pnl;
	frontend.ui->get_widget(pname, pnl);
	btn.signal_clicked.connect(sigc::bind(sigc::mem_fun(&frontend, &FrontEnd::show_panel), pnl));
}

void FrontEnd::Connector::start_series(GLtk::Button &btn, const string &name)
{
	btn.signal_clicked.connect(sigc::bind(sigc::mem_fun(&frontend, &FrontEnd::start_series), name));
}
