/* $Id$

This file is part of flipout
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cstdlib>
#include <cmath>
#include <msp/datafile/writer.h>
#include <msp/fs/dir.h>
#include <msp/fs/stat.h>
#include <msp/gl/clip.h>
#include <msp/gl/immediate.h>
#include <msp/gl/material.h>
#include <msp/gl/matrix.h>
#include <msp/gl/misc.h>
#include <msp/gl/projection.h>
#include <msp/gl/rendermode.h>
#include <msp/gl/select.h>
#include <msp/gl/tests.h>
#include <msp/gl/transform.h>
#include <msp/gltk/button.h>
#include <msp/gltk/list.h>
#include <msp/gltk/userinterface.h>
#include <msp/input/keys.h>
#include "bricktype.h"
#include "editor.h"
#include "flipout.h"
#include "game.h"
#include "level.h"
#include "vector3.h"

using namespace std;
using namespace Msp;

namespace {

float wrap(float v, float lo, float hi)
{
	while(v<lo)
		v+=hi-lo;
	while(v>hi)
		v-=hi-lo;
	return v;
}

}


Editor::Editor(FlipOut &fo):
	State(fo),
	level_dir(FS::get_user_data_dir("flipout")/"levels"),
	level(new Level),
	sel_btype(0),
	bx(0),
	by(0),
	bz(0),
	angle(0),
	pos_valid(false),
	camera(level->get_width(), level->get_height(), M_PI/8, flipout.get_window_aspect()),
	state(NORMAL)
{
	GLtk::UserInterface *ui=flipout.get_resources().get<GLtk::UserInterface>("editor");
	root=&ui->get_root();

	flipout.get_window().signal_key_press.connect(sigc::mem_fun(this, &Editor::key_press));
	flipout.get_window().signal_button_press.connect(sigc::mem_fun(this, &Editor::button_press));
	flipout.get_window().signal_pointer_motion.connect(sigc::mem_fun(this, &Editor::pointer_motion));

	GLtk::Dropdown *drp;
	ui->get_widget("drp_brick_types", drp);
	drp->clear();
	list<string> btypes=flipout.get_resources().get_names<BrickType>();
	for(list<string>::const_iterator i=btypes.begin(); i!=btypes.end(); ++i)
		drp->append(*i);
	drp->signal_item_selected.connect(sigc::mem_fun(this, &Editor::brick_type_selected));

	ui->get_widget("drp_brick_colors", drp_brick_colors);
	drp_brick_colors->clear();
	drp_brick_colors->signal_item_selected.connect(sigc::mem_fun(this, &Editor::brick_color_selected));

	ui->get_widget<GLtk::Button>("btn_exit").signal_clicked.connect(sigc::mem_fun(this, &Editor::exit));
	ui->get_widget<GLtk::Button>("btn_play").signal_clicked.connect(sigc::mem_fun(this, &Editor::play));
	ui->get_widget<GLtk::Button>("btn_save").signal_clicked.connect(sigc::mem_fun(this, &Editor::save));
	ui->get_widget<GLtk::Button>("btn_load").signal_clicked.connect(sigc::mem_fun(this, &Editor::load));
	ui->get_widget<GLtk::Button>("btn_new").signal_clicked.connect(sigc::mem_fun(this, &Editor::new_level));
	ui->get_widget<GLtk::Button>("btn_properties").signal_clicked.connect(sigc::mem_fun(this, &Editor::properties));

	ui->get_widget("pnl_filesel", pnl_filesel);
	ui->get_widget("lbl_filesel_title", lbl_filesel_title);

	ui->get_widget("lst_files", lst_files);
	lst_files->signal_item_selected.connect(sigc::mem_fun(this, &Editor::file_selected));

	ui->get_widget("ent_filename", ent_filename);
	ui->get_widget<GLtk::Button>("btn_filesel_ok").signal_clicked.connect(sigc::mem_fun(this, &Editor::filesel_ok));
	ui->get_widget<GLtk::Button>("btn_filesel_cancel").signal_clicked.connect(sigc::mem_fun(this, &Editor::filesel_cancel));

	ui->get_widget("pnl_properties", pnl_properties);
	ui->get_widget("lbl_properties_title", lbl_props_title);
	ui->get_widget("ent_width", ent_width);
	ui->get_widget("ent_height", ent_height);
	ui->get_widget("ent_title", ent_title);
	ui->get_widget<GLtk::Button>("btn_properties_ok").signal_clicked.connect(sigc::mem_fun(this, &Editor::properties_ok));
	ui->get_widget<GLtk::Button>("btn_properties_cancel").signal_clicked.connect(sigc::mem_fun(this, &Editor::properties_cancel));

	light.set_diffuse(GL::Color(0.8));
	light.set_position(0, -0.3, 1, 0);
	lighting.attach(0, light);

	camera.set_margins(0.15, 0);
}

void Editor::enable()
{
	state=NORMAL;
	root->set_visible(true);
}

void Editor::disable()
{
	state=INACTIVE;
	root->set_visible(false);
}

void Editor::tick()
{
}

void Editor::render() const
{
	camera.apply();

	lighting.bind();
	level->render(flipout.get_settings().use_shaders ? "" : "noshader");

	float lw=level->get_width();
	float lh=level->get_height();

	GL::Lighting::unbind();
	GL::translate(0, 0, 0.01);
	GL::Immediate imm((GL::COLOR4_UBYTE, GL::VERTEX2));
	imm.color(0.0f, 1.0f, 0.0f);
	imm.begin(GL::LINES);
	for(float x=ceil(-lw/2+0.001); x<lw/2; ++x)
	{
		imm.vertex(x, 0);
		imm.vertex(x, lh);
	}
	for(float y=1; y<lh; ++y)
	{
		imm.vertex(-lw/2, y);
		imm.vertex(lw/2, y);
	}
	imm.end();

	if(sel_btype && pos_valid)
	{
		float bw=sel_btype->get_width();
		float bh=sel_btype->get_height();

		GL::translate(bx, by, 0.01);
		GL::rotate(angle*180/M_PI, 0, 0, 1);

		imm.color(0.0f, 0.0f, 0.0f, 0.5f);
		imm.begin(GL::QUADS);
		imm.vertex(-bw/2, -bh/2);
		imm.vertex(bw/2, -bh/2);
		imm.vertex(bw/2, bh/2);
		imm.vertex(-bw/2, bh/2);
		imm.end();

		lighting.bind();
		GL::translate(0, 0, bz+sel_btype->get_depth()/2-0.02);

		GL::Color color;
		if(sel_bcolor!="random")
			color=sel_btype->get_color(sel_bcolor).color;
		color.a*=0.5;
		GL::Material mat;
		mat.set_diffuse(color);
		mat.set_ambient(color);
		mat.bind();

		sel_btype->render();
		GL::Material::unbind();
		GL::Lighting::unbind();
	}

	const GLtk::Geometry &root_geom=root->get_geometry();

	GL::matrix_mode(GL::PROJECTION);
	GL::load_identity();
	GL::ortho_bottomleft(root_geom.w, root_geom.h);
	GL::matrix_mode(GL::MODELVIEW);
	GL::load_identity();

	root->render();
	GL::Texture::unbind();
}

void Editor::key_press(unsigned code, unsigned, wchar_t)
{
	if(state!=NORMAL)
		return;

	unsigned key=Input::key_from_sys(code);

	if(key==Input::KEY_LEFT)
		angle=wrap(angle+M_PI/8, -M_PI, M_PI);
	else if(key==Input::KEY_RIGHT)
		angle=wrap(angle-M_PI/8, -M_PI, M_PI);
	else if(key==Input::KEY_UP)
		bz+=1;
	else if(key==Input::KEY_DOWN)
		bz=max(bz-1, 0.0f);
}

void Editor::button_press(int x, int y, unsigned btn, unsigned)
{
	if(state!=NORMAL)
		return;

	if(btn==1 && sel_btype)
	{
		if(pos_valid)
		{
			Level::BrickDef bdef(sel_btype);
			if(sel_bcolor=="random")
			{
				const ColorScheme &cs=sel_btype->get_color_scheme();
				map<string, BrickColor>::const_iterator i=cs.colors.begin();
				advance(i, rand()%cs.colors.size());
				bdef.color=i->first;
			}
			else
				bdef.color=sel_bcolor;
			bdef.x=bx;
			bdef.y=by;
			bdef.z=bz;
			bdef.angle=angle;
			level->add_brick(bdef);
		}
	}
	else if(btn==3)
	{
		float aspect=flipout.get_window_aspect();
		float xf=(x*1.0/flipout.get_window().get_width()-0.5)/0.85-0.075;
		float yf=(0.5-y*1.0/flipout.get_window().get_height())/0.85/aspect;
		float e=2.0/flipout.get_window().get_height();

		camera.apply();

		vector<GL::SelectRecord> sbuf;
		GL::select_buffer(sbuf);
		GL::render_mode(GL::SELECT);

		GL::push_matrix();
		GL::load_identity();
		GL::ClipPlane(1, 0, xf-e, 0).apply_to(0);
		GL::ClipPlane(-1, 0, -xf-e, 0).apply_to(1);
		GL::ClipPlane(0, 1, yf-e, 0).apply_to(2);
		GL::ClipPlane(0, -1, -yf-e, 0).apply_to(3);
		GL::pop_matrix();

		const list<Level::BrickDef> &bricks=level->get_bricks();
		for(list<Level::BrickDef>::const_iterator i=bricks.begin(); i!=bricks.end(); ++i)
		{
			GL::push_name(reinterpret_cast<unsigned>(&*i));
			GL::push_matrix();
			GL::translate(i->x, i->y, i->z+i->type->get_depth()*0.5);
			GL::rotate(i->angle*180/M_PI, 0, 0, 1);
			i->type->render("shadow");
			GL::pop_matrix();
			GL::pop_name();
		}

		GL::render_mode(GL::RENDER);
		GL::ClipPlane::disable(0);
		GL::ClipPlane::disable(1);
		GL::ClipPlane::disable(2);
		GL::ClipPlane::disable(3);

		if(!sbuf.empty())
		{
			const GL::SelectRecord *nearest=&sbuf.front();
			for(vector<GL::SelectRecord>::const_iterator i=sbuf.begin(); i!=sbuf.end(); ++i)
				if(i->min_depth<nearest->min_depth)
					nearest=&*i;
			level->remove_brick(*reinterpret_cast<Level::BrickDef *>(nearest->names.front()));
		}
	}
}

void Editor::pointer_motion(int x, int y)
{
	if(state!=NORMAL)
		return;

	int ww=flipout.get_window().get_width();
	int wh=flipout.get_window().get_height();
	Vector3 g=camera.get_ground_point(static_cast<float>(x*2-ww)/ww, static_cast<float>(wh-y*2)/wh);
	bx=round(g.x*2)/2;
	by=round(g.y*2)/2;

	if(sel_btype)
	{
		float lw=level->get_width();
		float lh=level->get_height();
		float bw=sel_btype->get_width();
		float bh=sel_btype->get_height();

		pos_valid=(bx>=(bw-lw)/2 && bx<=(lw-bw)/2 && by>=2+bh/2 && by<=lh-bh/2);
	}
}

void Editor::exit()
{
	finished=true;
}

void Editor::play()
{
	flipout.push_state(new Game(flipout, *level, string()));
}

void Editor::save()
{
	state=SAVE;
	populate_filesel();
	lbl_filesel_title->set_text("Save level");
	pnl_filesel->set_visible(true);
}

void Editor::load()
{
	state=LOAD;
	populate_filesel();
	lbl_filesel_title->set_text("Load level");
	pnl_filesel->set_visible(true);
}

void Editor::properties()
{
	state=PROPS;
	lbl_props_title->set_text("Level properties");
	ent_width->set_text(lexical_cast(level->get_width()));
	ent_height->set_text(lexical_cast(level->get_height()));
	ent_title->set_text(level->get_title());
	pnl_properties->set_visible(true);
}

void Editor::new_level()
{
	state=NEW;
	lbl_props_title->set_text("New level");
	ent_width->set_text("20");
	ent_height->set_text("15");
	ent_title->set_text("");
	pnl_properties->set_visible(true);
}

void Editor::brick_type_selected(unsigned index, const string &)
{
	list<BrickType *> btypes=flipout.get_resources().get_list<BrickType>();
	list<BrickType *>::iterator i=btypes.begin();
	advance(i, index);

	sel_btype=*i;

	signal_brick_type_selected.emit(sel_btype);

	drp_brick_colors->clear();
	const ColorScheme &cs=sel_btype->get_color_scheme();

	unsigned offset=0;
	if(cs.colors.size()>1)
	{
		drp_brick_colors->append("random");
		offset=1;
	}

	bool color_found=false;
	for(map<string, BrickColor>::const_iterator j=cs.colors.begin(); j!=cs.colors.end(); ++j)
	{
		drp_brick_colors->append(j->first);
		if(j->first==sel_bcolor)
		{
			drp_brick_colors->set_selected_index(distance(cs.colors.begin(), j)+offset);
			color_found=true;
		}
	}

	if(!color_found)
		drp_brick_colors->set_selected_index(0);
}

void Editor::brick_color_selected(unsigned, const string &str)
{
	sel_bcolor=str;
}

void Editor::populate_filesel()
{
	lst_files->clear();
	list<string> files=list_filtered(level_dir, "\\.level$");
	files.sort();
	for(list<string>::const_iterator i=files.begin(); i!=files.end(); ++i)
		lst_files->append(i->substr(0, i->size()-6));
}

void Editor::file_selected(unsigned, const string &fn)
{
	ent_filename->set_text(fn);
}

void Editor::filesel_cancel()
{
	state=NORMAL;
	pnl_filesel->set_visible(false);
}

void Editor::filesel_ok()
{
	FS::Path fn=level_dir/(ent_filename->get_text()+".level");

	if(state==SAVE)
	{
		if(!FS::exists(level_dir))
			FS::mkpath(level_dir, 0755);

		IO::BufferedFile out(fn.str(), IO::M_WRITE);
		out.write("/* FlipOut level */\n");

		DataFile::Writer writer(out);
		writer.set_binary(true);
		list<DataFile::Statement> st;
		level->save(st, flipout.get_resources());
		for(list<DataFile::Statement>::const_iterator i=st.begin(); i!=st.end(); ++i)
			writer.write(*i);
	}
	else if(state==LOAD)
	{
		RefPtr<Level> new_lvl=new Level;
		DataFile::load(*new_lvl, fn.str(), flipout.get_resources());

		delete level;
		level=new_lvl.release();
		camera.set_size(level->get_width(), level->get_height());
	}

	state=NORMAL;
	pnl_filesel->set_visible(false);
}

void Editor::properties_cancel()
{
	state=NORMAL;
	pnl_properties->set_visible(false);
}

void Editor::properties_ok()
{
	float width;
	float height;

	try
	{
		width=lexical_cast<float>(ent_width->get_text());
		height=lexical_cast<float>(ent_height->get_text());
	}
	catch(const LexicalError &)
	{
		return;
	}

	string title=ent_title->get_text();

	if(state==NEW)
	{
		RefPtr<Level> new_lvl=new Level;
		new_lvl->set_size(width, height);
		new_lvl->set_title(title);

		delete level;
		level=new_lvl.release();
	}
	else if(state==PROPS)
	{
		level->set_size(width, height);
		level->set_title(title);
	}

	camera.set_size(level->get_width(), level->get_height());

	state=NORMAL;
	pnl_properties->set_visible(false);
}
