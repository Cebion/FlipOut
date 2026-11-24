/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/time/units.h>
#include "balltype.h"
#include "bonusaction.h"
#include "bonustype.h"
#include "playfield.h"
#include "resources.h"

using namespace std;
using namespace Msp;

BonusType::BonusType():
	probability(1)
{ }

BonusType::~BonusType()
{
	for(list<BonusAction *>::iterator i=actions.begin(); i!=actions.end(); ++i)
		delete *i;
}

void BonusType::run_actions(Playfield &pf, Bonus &bonus) const
{
	/* XXX Previous action of the same type won't be terminated - it is assumed
	   that the new one overrides it.  Works fine for now, but might need to be
	   futureproofed. */
	for(list<BonusAction *>::const_iterator i=actions.begin(); i!=actions.end(); ++i)
		if((*i)->run(pf, bonus) && duration)
			pf.add_effect(*this, **i);
}


BonusType::Loader::Loader(BonusType &bt, Collection &c):
	Object::Loader(bt, c),
	btype(bt)
{
	add("ball_magnet", &Loader::ball_magnet);
	add("collect_sound", &BonusType::collect_sound);
	add("fill_bonus",  &Loader::fill_bonus);
	add("duration",    &Loader::duration);
	add("extra_life",  &Loader::extra_life);
	add("friction",    &Loader::friction);
	add("gravity",     &Loader::gravity);
	add("new_ball",    &Loader::new_ball);
	add("probability", &BonusType::probability);
	add("quake",       &Loader::quake);
	add("score",       &Loader::score);
	add("text",        &Loader::text);
	add("unlimited_balls", &Loader::unlimited_balls);
}

void BonusType::Loader::ball_magnet(float f)
{
	btype.actions.push_back(new BallMagnetAction(f));
}

void BonusType::Loader::fill_bonus(const string &n)
{
	btype.actions.push_back(new FillBonusAction(coll.get<BonusType>(n)));
}

void BonusType::Loader::duration(float d)
{
	btype.duration=d*Msp::Time::sec;
}

void BonusType::Loader::extra_life()
{
	btype.actions.push_back(new ExtraLifeAction());
}

void BonusType::Loader::friction(float f)
{
	btype.actions.push_back(new FrictionAction(f));
}

void BonusType::Loader::gravity(float g)
{
	btype.actions.push_back(new GravityAction(g));
}

void BonusType::Loader::new_ball(const string &t)
{
	btype.actions.push_back(new NewBallAction(*coll.get<BallType>(t)));
}

void BonusType::Loader::quake(float q)
{
	btype.actions.push_back(new QuakeAction(q));
}

void BonusType::Loader::score(unsigned s)
{
	btype.actions.push_back(new ScoreAction(s));
}

void BonusType::Loader::text(const string &t)
{
	btype.actions.push_back(new TextAction(t));
}

void BonusType::Loader::unlimited_balls()
{
	btype.actions.push_back(new UnlimitedBallsAction);
}
