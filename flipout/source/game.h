/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef GAME_H_
#define GAME_H_

#include <msp/gl/font.h>
#include <msp/gltk/entry.h>
#include <msp/gltk/label.h>
#include <msp/gltk/panel.h>
#include <msp/gltk/root.h>
#include <msp/time/timer.h>
#include "state.h"

class Level;
class Playfield;
class Series;
class SplashText;

/**
This state is used for playing the game.  It can be used to play a single Level
or a Series.
*/
class Game: public State, public sigc::trackable
{
private:
	enum GameState
	{
		PLAYING,
		AWARDS,
		PAUSED,
		FINISHED,
		GAME_OVER
	};

	const Series *series;
	unsigned series_pos;
	Playfield *playfield;
	unsigned score;
	unsigned lives;
	unsigned next_life;
	GameState state;
	Msp::GLtk::UserInterface *ui;
	Msp::GLtk::Root *ui_root;
	Msp::GLtk::Label *lbl_score;
	Msp::GLtk::Label *lbl_level;
	Msp::GLtk::Label *lbl_lives;
	Msp::GLtk::Label *lbl_framerate;
	Msp::GLtk::Label *lbl_tickperf;
	Msp::GLtk::Label *lbl_objects;
	Msp::GLtk::Label *lbl_contacts;
	Msp::GLtk::Panel *pnl_entername;
	Msp::GLtk::Entry *ent_name;
	Msp::Time::TimeDelta stepsize;
	Msp::Time::TimeStamp next_step;
	unsigned prev_steps;
	SplashText *splash_text;
	std::string score_id;
	bool slow_motion;

public:
	Game(FlipOut &, const Series &, const std::string &);
	Game(FlipOut &, const Level &, const std::string &);
private:
	void init();
public:
	~Game();

	virtual void enable();
	virtual void tick();
	virtual void render() const;
	void toggle_pause();
	void toggle_slow_motion();
	void exit();
	void add_score(unsigned);
private:
	bool lost_life();
	void extra_life();
	void level_finished();
	void set_state(GameState);
	void start_level(const Level &lvl);
	bool next_level();
	bool show_awards();
	bool game_over();
	void name_entered();
};

#endif
