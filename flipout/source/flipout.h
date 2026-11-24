/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef FLIPOUT_H_
#define FLIPOUT_H_

#include <msp/al/context.h>
#include <msp/al/device.h>
#include <msp/core/application.h>
#include <msp/debug/profiler.h>
#include <msp/fs/path.h>
#include <msp/gbase/display.h>
#include <msp/gbase/glcontext.h>
#include <msp/gbase/window.h>
#include <msp/input/hub.h>
#include <msp/input/keyboard.h>
#include <msp/input/mouse.h>
#include <msp/time/timer.h>
#include <msp/time/timestamp.h>
#include "controls.h"
#include "highscores.h"
#include "resources.h"
#include "settings.h"

class Music;
class State;

class FlipOut: public Msp::Application
{
public:
	FlipOut(int, char **);
	~FlipOut();
	int main();

	Resources &get_resources() { return resources; }
	Settings &get_settings() { return settings; }
	Controls &get_controls() { return controls; }
	HighScores &get_highscores() { return highscores; }
	const Msp::Graphics::Display &get_display() const { return *display; }
	Msp::Graphics::Window &get_window() const { return *window; }
	Msp::Input::Hub &get_hub() const { return *hub; }
	unsigned get_window_width() const { return window->get_width(); }
	unsigned get_window_height() const { return window->get_height(); }
	float get_window_aspect() const;
	Music &get_music() const { return *music; }
	void push_state(State *);
	Msp::Time::Timer::Slot &add_timer(const Msp::Time::TimeDelta &);
	Msp::Debug::Profiler &get_profiler() { return profiler; }
	bool get_debug() const { return debug; }
private:
	Msp::Graphics::Display *display;
	Msp::Graphics::Window *window;
	Msp::Graphics::GLContext *glctx;
	Msp::Input::Keyboard *kbd;
	Msp::Input::Mouse *mouse;
	Msp::Input::Hub *hub;
	Msp::AL::Device *al_dev;
	Msp::AL::Context *alctx;
	Music *music;
	Settings settings;
	Controls controls;
	Msp::FS::Path data_dir;
	Resources resources;
	HighScores highscores;
	std::list<State *> states;
	std::string play_level;
	Msp::Debug::Profiler profiler;
	Msp::Time::Timer timer;
	bool debug;
	bool start_editor;
	bool ode_init_done;

	void tick();

	static Msp::Application::RegApp<FlipOut> reg;
};

#endif
