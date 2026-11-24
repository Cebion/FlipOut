/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cstdlib>
#include <iostream>
#include <sigc++/bind.h>
#include <ode/ode.h>
#include <msp/core/getopt.h>
#include <msp/debug/profilingscope.h>
#include <msp/gl/blend.h>
#include <msp/gl/immediate.h>
#include <msp/gl/matrix.h>
#include <msp/gl/misc.h>
#include <msp/gl/projection.h>
#include <msp/gl/tests.h>
#include <msp/gl/transform.h>
#include <msp/input/keys.h>
#include <msp/fs/dir.h>
#include <msp/fs/stat.h>
#include <msp/strings/regex.h>
#include <msp/time/units.h>
#include <msp/time/utils.h>
#include "editor.h"
#include "flipout.h"
#include "frontend.h"
#include "game.h"
#include "level.h"
#include "music.h"
#include "series.h"

using namespace std;
using namespace Msp;

FlipOut::FlipOut(int argc, char **argv):
	display(0),
	window(0),
	glctx(0),
	kbd(0),
	mouse(0),
	hub(0),
	al_dev(0),
	alctx(0),
	music(0),
	data_dir(FS::get_sys_data_dir(argv[0], "flipout")),
	resources(*this),
	debug(false),
	start_editor(false),
	ode_init_done(false)
{
	GetOpt getopt;
	getopt.add_option('e', "editor", start_editor, GetOpt::NO_ARG);
	getopt.add_option('g', "debug",  debug,  GetOpt::NO_ARG);
	getopt(argc, argv);

	const vector<string> &args=getopt.get_args();
	if(!args.empty())
		play_level=args.front();

	profiler.set_period(100);
	profiler.add_scope("frame");
	profiler.add_scope("tick");
	profiler.add_scope("collide");
	profiler.add_scope("step");
	profiler.add_scope("render");
}

FlipOut::~FlipOut()
{
	if(ode_init_done)
		dCloseODE();
	delete music;
	delete alctx;
	delete al_dev;
	delete hub;
	delete mouse;
	delete kbd;
	delete glctx;
	delete window;
	delete display;
}

int FlipOut::main()
{
	settings.load();

	srand(time(0));

	display=new Graphics::Display;

	Graphics::WindowOptions wopt;
	wopt.width=settings.width;
	wopt.height=settings.height;
	wopt.fullscreen=settings.fullscreen;
	window=new Graphics::Window(*display, wopt);
	window->set_title("FlipOut");
	window->signal_close.connect(sigc::bind(sigc::mem_fun(this, &FlipOut::exit), 0));
	window->show();

	Graphics::GLOptions glopt;
	glopt.alpha=true;

	glctx=new Graphics::GLContext(*window, glopt);

	kbd=new Input::Keyboard(*window);
	mouse=new Input::Mouse(*window);
	hub=new Input::Hub;
	hub->attach(*kbd);
	hub->attach(*mouse);

	controls.move_left.set_source(*hub, Input::BUTTON, Input::KEY_LEFT);
	controls.move_right.set_source(*hub, Input::BUTTON, Input::KEY_RIGHT);
	controls.turbo.set_source(*hub, Input::BUTTON, Input::KEY_SHIFT_L);
	controls.flip_left.set_source(*hub, Input::BUTTON, Input::KEY_X);
	controls.flip_right.set_source(*hub, Input::BUTTON, Input::KEY_Z);
	controls.pause.set_source(*hub, Input::BUTTON, Input::KEY_PAUSE);
	controls.slow_motion.set_source(*hub, Input::BUTTON, Input::KEY_BACKSPACE);
	controls.exit.set_source(*hub, Input::BUTTON, Input::KEY_ESC);

	for(map<string, unsigned>::const_iterator i=settings.controls.begin(); i!=settings.controls.end(); ++i)
		controls.get(i->first).set_source(*hub, static_cast<Input::ControlSrcType>(i->second>>24), i->second&0xFFFFFF);

	al_dev=new AL::Device;
	alctx=new AL::Context(*al_dev);
	music=new Music;

	if(FS::is_reg(data_dir/"flipout.data"))
		DataFile::load(resources, (data_dir/"flipout.data").str());
	if(FS::is_dir(data_dir/"data"))
		resources.load(data_dir/"data");

	if(settings.vertex_buffers)
	{
		list<GL::Mesh *> meshes=resources.get_list<GL::Mesh>();
		for(list<GL::Mesh *>::iterator i=meshes.begin(); i!=meshes.end(); ++i)
			(*i)->use_vertex_buffer(true);
	}

	highscores.load();

	GL::enable(GL::DEPTH_TEST);
	GL::depth_func(GL::LEQUAL);
	GL::enable(GL::BLEND);
	GL::blend_func(GL::SRC_ALPHA, GL::ONE_MINUS_SRC_ALPHA);
	GL::enable(GL_CULL_FACE);

	dInitODE();
	ode_init_done=true;

	if(start_editor)
		states.push_back(new Editor(*this));
	else if(!play_level.empty())
		states.push_back(new Game(*this, *resources.get<Level>(play_level), play_level));
	else
		states.push_back(new FrontEnd(*this));

	set_loop_mode(TICK_BUSY);

	music->set_volume(settings.music_volume);
	if(settings.music_enabled)
		music->play();

	return Application::main();
}

float FlipOut::get_window_aspect() const
{
	return float(window->get_width())/window->get_height();
}

void FlipOut::push_state(State *s)
{
	if(!states.empty())
		states.back()->disable();

	states.push_back(s);
	s->enable();
}

Msp::Time::Timer::Slot &FlipOut::add_timer(const Msp::Time::TimeDelta &dt)
{
	return timer.add(dt);
}

void FlipOut::tick()
{
	Debug::ProfilingScope _s(profiler, "frame");

	{
		Debug::ProfilingScope _t(profiler, "tick");

		display->tick();
		music->tick();
		timer.tick(false);
		states.back()->tick();
	}

	{
		Debug::ProfilingScope _t(profiler, "render");

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		list<State *>::iterator i=states.end();
		for(--i; (i!=states.begin() && !(*i)->get_opaque()); --i) ;
		for(; i!=states.end(); ++i)
			(*i)->render();
	}

	{
		Debug::ProfilingScope _t(profiler, "swap");
		glctx->swap_buffers();
	}

	while(!states.empty() && states.back()->get_finished())
	{
		delete states.back();
		states.erase(--states.end());

		if(states.empty())
			exit(0);
		else
			states.back()->enable();
	}
}

Msp::Application::RegApp<FlipOut> FlipOut::reg;
