/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef BONUSTYPE_H_
#define BONUSTYPE_H_

#include <msp/al/buffer.h>
#include <msp/gl/object.h>
#include <msp/time/timedelta.h>

class Bonus;
class BonusAction;
class Playfield;

class BonusType: public Msp::GL::Object
{
public:
	class Loader: public Msp::GL::Object::Loader
	{
	private:
		BonusType &btype;
	
	public:
		Loader(BonusType &, Collection &);
		BonusType &get_object() const { return btype; }
	private:
		void ball_magnet(float);
		void fill_bonus(const std::string &);
		void duration(float);
		void extra_life();
		void friction(float);
		void gravity(float);
		void new_ball(const std::string &);
		void quake(float);
		void score(unsigned);
		void text(const std::string &);
		void unlimited_balls();
	};

private:
	std::list<BonusAction *> actions;
	Msp::Time::TimeDelta duration;
	float probability;
	const Msp::AL::Buffer *collect_sound;

public:
	BonusType();
	~BonusType();

	const Msp::Time::TimeDelta &get_duration() const { return duration; }
	float get_probability() const { return probability; }
	const Msp::AL::Buffer *get_collect_sound() const { return collect_sound; }

	void run_actions(Playfield &, Bonus &) const;
};

#endif
