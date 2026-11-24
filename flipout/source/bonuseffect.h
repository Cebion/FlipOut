/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef BONUSEFFECT_H_
#define BONUSEFFECT_H_

#include <msp/time/timedelta.h>

class BonusType;
class BonusAction;

class BonusEffect
{
private:
	Playfield &playfield;
	const BonusType &btype;
	const BonusAction &action;
	Msp::Time::TimeDelta duration;
	Msp::Time::TimeDelta visible_duration;
	float display_angle;
	float slide_pos;

public:
	BonusEffect(Playfield &, const BonusType &, const BonusAction &);
	~BonusEffect();

	void extend(const Msp::Time::TimeDelta &);
	bool is_stale() const;
	const BonusType &get_type() const { return btype; }
	const BonusAction &get_action() const { return action; }
	const Msp::Time::TimeDelta &get_duration() const { return duration; }
	void step(const Msp::Time::TimeDelta &);
	void render() const;
};

#endif
