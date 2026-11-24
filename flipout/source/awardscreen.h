/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef AWARDSCREEN_H_
#define AWARDSCREEN_H_

#include <sigc++/trackable.h>
#include <msp/gltk/label.h>
#include <msp/gltk/panel.h>
#include <msp/time/timestamp.h>
#include "state.h"

class AwardScreen: public State, public sigc::trackable
{
private:
	struct AwardInfo
	{
		unsigned score;
		Msp::GLtk::Label *lbl_name;
		Msp::GLtk::Label *lbl_score;
	};

	Msp::GLtk::Panel *pnl_awards;
	Msp::GLtk::Label *lbl_total_score;
	Msp::GLtk::Geometry panel_geom;
	unsigned total_score;
	std::list<AwardInfo> awards;
	std::list<AwardInfo>::iterator cur_award;
	unsigned total_awards;
	unsigned award_score;
	Msp::Time::TimeStamp last_tick;
	float speed;

public:
	AwardScreen(FlipOut &, unsigned, const std::map<std::string, unsigned> &);
	~AwardScreen();

	virtual void tick();
	virtual void render() const;
private:
	bool start();
	bool next();
	bool finish();
};

#endif
