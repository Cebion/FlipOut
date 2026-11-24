/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "ball.h"
#include "bonus.h"
#include "bonusaction.h"
#include "flipout.h"
#include "floatingtext.h"
#include "playfield.h"

using namespace std;
using namespace Msp;

ScoreAction::ScoreAction(unsigned s):
	score(s)
{ }

bool ScoreAction::run(Playfield &pf, const Bonus &) const
{
	pf.signal_score.emit(score);

	return false;
}


NewBallAction::NewBallAction(const BallType &t):
	type(t)
{ }

bool NewBallAction::run(Playfield &pf, const Bonus &bonus) const
{
	Ball *ball=new Ball(pf, type);
	ball->set_position(bonus.get_position());
	ball->set_linear_velocity(bonus.get_linear_velocity());

	return false;
}


bool ExtraLifeAction::run(Playfield &pf, const Bonus &) const
{
	pf.signal_extra_life.emit();

	return false;
}


GravityAction::GravityAction(float m):
	modifier(m)
{ }

bool GravityAction::run(Playfield &pf, const Bonus &) const
{
	pf.set_gravity_modifier(modifier);

	return true;
}

void GravityAction::terminate(Playfield &pf) const
{
	pf.set_gravity_modifier(1);
}


FrictionAction::FrictionAction(float m):
	modifier(m)
{ }

bool FrictionAction::run(Playfield &pf, const Bonus &) const
{
	pf.set_friction_modifier(modifier);

	return true;
}

void FrictionAction::terminate(Playfield &pf) const
{
	pf.set_friction_modifier(1);
}


FillBonusAction::FillBonusAction(const BonusType *bt):
	btype(bt)
{ }

bool FillBonusAction::run(Playfield &pf, const Bonus &) const
{
	pf.set_fill_bonus(btype);

	return true;
}

void FillBonusAction::terminate(Playfield &pf) const
{
	pf.set_fill_bonus(0);
}


BallMagnetAction::BallMagnetAction(float f):
	force(f)
{ }

bool BallMagnetAction::run(Playfield &pf, const Bonus &) const
{
	pf.set_ball_magnet(force);

	return true;
}

void BallMagnetAction::terminate(Playfield &pf) const
{
	pf.set_ball_magnet(0);
}


bool UnlimitedBallsAction::run(Playfield &pf, const Bonus &) const
{
	pf.set_unlimited_balls(true);

	return true;
}

void UnlimitedBallsAction::terminate(Playfield &pf) const
{
	pf.set_unlimited_balls(false);
}


TextAction::TextAction(const string &t):
	text(t)
{ }

bool TextAction::run(Playfield &pf, const Bonus &bonus) const
{
	FloatingText *ftxt=new FloatingText(pf, *pf.get_flipout().get_resources().get_ui_resources().get<GLtk::Style>("float")->get_font(), text, GL::Color());
	ftxt->set_position(bonus.get_position());

	return false;
}


QuakeAction::QuakeAction(float m):
	strength(m)
{ }

bool QuakeAction::run(Playfield &pf, const Bonus &) const
{
	pf.set_quake(strength);

	return true;
}

void QuakeAction::terminate(Playfield &pf) const
{
	pf.set_quake(0);
}
