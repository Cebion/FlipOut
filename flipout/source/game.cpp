/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/immediate.h>
#include <msp/gl/matrix.h>
#include <msp/gl/misc.h>
#include <msp/gl/projection.h>
#include <msp/gl/tests.h>
#include <msp/gl/transform.h>
#include <msp/gltk/button.h>
#include <msp/gltk/userinterface.h>
#include <msp/strings/formatter.h>
#include <msp/strings/lexicalcast.h>
#include <msp/time/units.h>
#include <msp/time/utils.h>
#include "awardscreen.h"
#include "background.h"
#include "ball.h"
#include "balltype.h"
#include "game.h"
#include "flipout.h"
#include "level.h"
#include "paddle.h"
#include "paddletype.h"
#include "playfield.h"
#include "resources.h"
#include "series.h"
#include "splashtext.h"

using namespace std;
using namespace Msp;

Game::Game(FlipOut &fo, const Series &s, const string &si):
	State(fo),
	series(&s),
	series_pos(0),
	score_id(si)
{
	init();

	start_level(series->get_level(series_pos));
}

Game::Game(FlipOut &fo, const Level &lvl, const string &si):
	State(fo),
	series(0),
	score_id(si)
{
	init();

	start_level(lvl);
}

void Game::init()
{
	score=0;
	lives=7-flipout.get_settings().difficulty*2;
	next_life=5000;
	for(unsigned i=0; i<flipout.get_settings().difficulty; ++i)
		next_life*=2;

	ui=flipout.get_resources().get<GLtk::UserInterface>("game");
	ui_root=&ui->get_root();
	
	ui->get_widget("lbl_score", lbl_score);
	ui->get_widget("lbl_level", lbl_level);
	ui->get_widget("lbl_lives", lbl_lives);
	ui->get_widget("lbl_framerate", lbl_framerate);
	ui->get_widget("lbl_tickperf", lbl_tickperf);
	ui->get_widget("lbl_objects", lbl_objects);
	ui->get_widget("lbl_contacts", lbl_contacts);
	ui->get_widget("pnl_entername", pnl_entername);
	ui->get_widget("ent_name", ent_name);
	lbl_score->set_text(lexical_cast(score));
	lbl_lives->set_text(format("Lives: %d", lives));

	flipout.get_controls().pause.signal_press.connect(sigc::mem_fun(this, &Game::toggle_pause));
	flipout.get_controls().slow_motion.signal_press.connect(sigc::mem_fun(this, &Game::toggle_slow_motion));
	flipout.get_controls().exit.signal_press.connect(sigc::mem_fun(this, &Game::exit));

	static unsigned stepsizes[]={ 20, 14, 10, 7, 5 };

	stepsize=stepsizes[flipout.get_settings().physics_quality]*Msp::Time::msec;
	prev_steps=0;

	splash_text=0;
	slow_motion=false;

	pnl_entername->set_visible(false);
	ui->get_widget<GLtk::Panel>("pnl_perfdata").set_visible(flipout.get_debug());

	flipout.get_window().show_cursor(false);
}

Game::~Game()
{
	delete playfield;
	delete splash_text;

	flipout.get_window().show_cursor(true);
}

void Game::enable()
{
	if(state==AWARDS)
	{
		const map<string, unsigned> &awards=playfield->get_awards();
		for(map<string, unsigned>::const_iterator i=awards.begin(); i!=awards.end(); ++i)
			add_score(i->second);
		next_level();
	}
}

void Game::tick()
{
	const Msp::Time::TimeStamp t=Msp::Time::now();

	if(state==PLAYING)
	{
		unsigned nsteps=0;
		while(state==PLAYING && (next_step<t || nsteps+1<prev_steps) && nsteps<prev_steps+1 && nsteps<5)
		{
			playfield->step(stepsize);
			if(slow_motion)
				next_step+=stepsize*25;
			else
				next_step+=stepsize;
			++nsteps;
		}
		prev_steps=nsteps;

		Msp::Time::TimeDelta tick_time=flipout.get_profiler().scope("tick").avg_time;
		Msp::Time::TimeDelta coll_time=flipout.get_profiler().scope("collide").avg_time;
		Msp::Time::TimeDelta step_time=flipout.get_profiler().scope("step").avg_time;
		Msp::Time::TimeDelta rend_time=flipout.get_profiler().scope("render").avg_time;
		Msp::Time::TimeDelta total=tick_time+rend_time;
		Msp::Time::TimeDelta phys_total=coll_time+step_time;
		double rend_slice=rend_time/total;
		double tick_slice=tick_time/total;
		double coll_slice=(phys_total ? coll_time/phys_total*tick_slice : 0);
		double step_slice=(phys_total ? step_time/phys_total*tick_slice : 0);

		lbl_framerate->set_text(format("Frametime: %s", flipout.get_profiler().scope("frame").avg_time));
		lbl_tickperf->set_text(format("Coll/Step/Rend: %2.0f%%/%2.0f%%/%2.0f%%", coll_slice*100, step_slice*100, rend_slice*100));
		lbl_objects->set_text(format("Objects: %d", playfield->get_n_objects()));
		lbl_contacts->set_text(format("Contacts: %d", playfield->get_n_contacts()));
	}
	else
	{
		if(splash_text)
			splash_text->tick(t-next_step);

		next_step=t;
	}
}

void Game::render() const
{
	playfield->render();

	const GLtk::Geometry &root_geom=ui_root->get_geometry();
	GL::matrix_mode(GL::PROJECTION);
	GL::load_identity();
	GL::ortho_bottomleft(root_geom.w, root_geom.h);
	GL::matrix_mode(GL::MODELVIEW);
	GL::load_identity();
	GL::disable(GL::DEPTH_TEST);

	if(state!=PLAYING)
	{
		GL::Texture::unbind();
		GL::Immediate imm((GL::COLOR4_UBYTE,GL::VERTEX2));
		imm.begin(GL::QUADS);
		imm.color(0.0f, 0.0f, 0.0f, 0.5f);
		imm.vertex(0, 0);
		imm.vertex(root_geom.w, 0);
		imm.vertex(root_geom.w, root_geom.h);
		imm.vertex(0, root_geom.h);
		imm.end();
	}

	ui_root->render();

	if(state!=PLAYING)
	{
		if(splash_text)
		{
			const Msp::GL::Font &font=*flipout.get_resources().get_ui_resources().get<GLtk::Style>("splash")->get_font();

			GL::push_matrix();
			GL::translate(root_geom.w/2, root_geom.h/2, 0);
			GL::scale_uniform(font.get_default_size()*2);
			splash_text->render();
			GL::pop_matrix();
		}

		glColor4f(1, 1, 1, 1);
	}

	GL::Texture::unbind();
	GL::enable(GL::DEPTH_TEST);
}

void Game::toggle_pause()
{
	if(state==PLAYING)
	{
		set_state(PAUSED);
		splash_text=new SplashText("PAUSED", *flipout.get_resources().get_ui_resources().get<GLtk::Style>("splash")->get_font());
	}
	else if(state==PAUSED)
	{
		set_state(PLAYING);
		delete splash_text;
		splash_text=0;
	}
}

void Game::toggle_slow_motion()
{
	if(flipout.get_debug())
		slow_motion=!slow_motion;
}

void Game::exit()
{
	if(state==PLAYING || state==PAUSED)
		finished=true;
}

void Game::add_score(unsigned s)
{
	score+=(s*(3+flipout.get_settings().difficulty)+3)/4;
	lbl_score->set_text(lexical_cast(score));

	if(score>next_life)
	{
		extra_life();
		unsigned increment=5000;
		for(unsigned i=0; i<flipout.get_settings().difficulty; ++i)
			increment*=2;
		next_life+=increment;
	}
}

bool Game::lost_life()
{
	--lives;
	lbl_lives->set_text(format("Lives: %d", lives));
	if(lives==0)
	{
		set_state(GAME_OVER);
		splash_text=new SplashText("GAME OVER", *flipout.get_resources().get_ui_resources().get<GLtk::Style>("splash")->get_font());
		splash_text->set_color(GL::Color(1.0, 0.2, 0.0));

		flipout.add_timer(4.0*Msp::Time::sec).signal_timeout.connect(sigc::mem_fun(this, &Game::game_over));
	}

	return lives>0;
}

void Game::extra_life()
{
	++lives;
	lbl_lives->set_text(format("Lives: %d", lives));
}

void Game::level_finished()
{
	set_state(FINISHED);
	splash_text=new SplashText("CLEAR!", *flipout.get_resources().get_ui_resources().get<GLtk::Style>("splash")->get_font());
	splash_text->set_color(GL::Color(0.7, 0.8, 1.0));
	flipout.add_timer(2.0*Msp::Time::sec).signal_timeout.connect(sigc::mem_fun(this, playfield->get_awards().empty() ? &Game::next_level : &Game::show_awards));
}

void Game::set_state(GameState s)
{
	state=s;

	next_step=Msp::Time::now();
}

void Game::start_level(const Level &lvl)
{
	playfield=lvl.create_playfield(flipout);

	new Paddle(*playfield, *flipout.get_resources().get<PaddleType>("normal paddle"));

	playfield->place_ball(*new Ball(*playfield, *flipout.get_resources().get<BallType>("normal ball")));

	const list<string> &bgs=flipout.get_resources().get_names<Background>();
	if(!bgs.empty())
	{
		list<string>::const_iterator i=bgs.begin();
		advance(i, rand()%bgs.size());
		playfield->set_background(flipout.get_resources().get<Background>(*i));
	}

	playfield->signal_score.connect(sigc::mem_fun(this, &Game::add_score));
	playfield->signal_lost_life.connect(sigc::mem_fun(this, &Game::lost_life));
	playfield->signal_finished.connect(sigc::mem_fun(this, &Game::level_finished));
	playfield->signal_extra_life.connect(sigc::mem_fun(this, &Game::extra_life));

	const string &title=lvl.get_title();
	if(title.empty())
	{
		if(series)
			lbl_level->set_text(format("Level %d of %d", series_pos+1, series->get_n_levels()));
		else
			lbl_level->set_text("Single level");
	}
	else if(series)
		lbl_level->set_text(format("%s (%d of %d)", title, series_pos+1, series->get_n_levels()));
	else
		lbl_level->set_text(title);

	set_state(PLAYING);
}

bool Game::next_level()
{
	if(series && series_pos+1<series->get_n_levels())
	{
		++series_pos;
		delete playfield;
		lbl_level->set_text(format("Level %d of %d", series_pos+1, series->get_n_levels()));
		start_level(series->get_level(series_pos));
	}
	else
		game_over();

	return false;
}

bool Game::show_awards()
{
	set_state(AWARDS);
	delete splash_text;
	splash_text=0;
	flipout.push_state(new AwardScreen(flipout, score, playfield->get_awards()));

	return false;
}

bool Game::game_over()
{
	delete splash_text;
	splash_text=0;

	if(!score_id.empty() && flipout.get_highscores().get_ranking(score_id, score)<=10)
	{
		pnl_entername->set_visible(true);
		ent_name->signal_enter.connect(sigc::mem_fun(this, &Game::name_entered));
		ent_name->set_focus();
		ui->get_widget<GLtk::Button>("btn_name_ok").signal_clicked.connect(sigc::mem_fun(this, &Game::name_entered));

		flipout.get_window().show_cursor(true);
	}
	else
		finished=true;

	return false;
}

void Game::name_entered()
{
	flipout.get_highscores().add_score(score_id, score, (series ? series_pos+1 : 0), ent_name->get_text());
	finished=true;
}
