/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cmath>
#include <msp/gl/immediate.h>
#include <msp/gl/lighting.h>
#include <msp/gl/matrix.h>
#include <msp/gl/misc.h>
#include <msp/gl/transform.h>
#include <msp/time/units.h>
#include "bonusaction.h"
#include "bonuseffect.h"
#include "bonustype.h"

using namespace std;
using namespace Msp;

BonusEffect::BonusEffect(Playfield &pf, const BonusType &bt, const BonusAction &a):
	playfield(pf),
	btype(bt),
	action(a),
	duration(btype.get_duration()),
	display_angle(0),
	slide_pos(1)
{ }

BonusEffect::~BonusEffect()
{
	/*if(!is_stale())
		action.terminate(playfield);*/
}

void BonusEffect::extend(const Time::TimeDelta &t)
{
	duration+=t;
}

bool BonusEffect::is_stale() const
{
	return duration<=Time::zero;
}

void BonusEffect::step(const Time::TimeDelta &stepsize)
{
	if(!is_stale())
	{
		duration-=stepsize;
		if(is_stale())
			action.terminate(playfield);
	}

	if(visible_duration<duration-Time::sec/2)
		visible_duration+=stepsize*60;
	else
		visible_duration=duration;

	display_angle+=60*(stepsize/Time::sec);

	if(duration<0.5*Time::sec)
		slide_pos+=2*(stepsize/Time::sec);
	else if(slide_pos>0)
		slide_pos-=4*(stepsize/Time::sec);
}

void BonusEffect::render() const
{
	GL::push_matrix();

	GL::translate(slide_pos*3, 0, 0);

	GL::push_matrix();
	GL::rotate(display_angle, 1, 0, 0);

	btype.render();

	GL::disable(GL::LIGHTING);
	GL::pop_matrix();

	GL::translate(-1.3, 0, 0);

	double time=min(visible_duration/Time::sec, 60.0);

	GL::Immediate imm((GL::COLOR4_UBYTE, GL::VERTEX2));
	imm.begin(GL::TRIANGLE_FAN);
	imm.color(0.8f, 0.5f, 0.0f, 0.8f);
	imm.vertex(0, 0);
	imm.vertex(cos((15-time)*M_PI/30)*0.7, sin((15-time)*M_PI/30)*0.7);
	for(int i=static_cast<int>(time); i>=0; --i)
		imm.vertex(cos((15-i)*M_PI/30)*0.7, sin((15-i)*M_PI/30)*0.7);
	imm.end();

	imm.begin(GL::TRIANGLE_FAN);
	imm.color(0.8f, 0.8f, 0.8f, 0.5f);
	imm.vertex(0, 0);
	for(int i=60; i>=time; --i)
		imm.vertex(cos((15-i)*M_PI/30)*0.7, sin((15-i)*M_PI/30)*0.7);
	imm.vertex(cos((15-time)*M_PI/30)*0.7, sin((15-time)*M_PI/30)*0.7);
	imm.end();

	GL::enable(GL::LIGHTING);
	GL::pop_matrix();
}
