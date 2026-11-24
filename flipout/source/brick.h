/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef BRICK_H_
#define BRICK_H_

#include <ode/ode.h>
#include <msp/gl/material.h>
#include <msp/gl/objectinstance.h>
#include "dynamicobject.h"
#include "vector3.h"

class BonusType;
class BrickColor;
class BrickType;
class Playfield;

class Brick: public DynamicObject, public Msp::GL::ObjectInstance
{
private:
	const BrickType &type;
	unsigned durability;
	Msp::GL::Material material;
	const BonusType *bonus_type;
	Msp::GL::ProgramData *shdata;
	Msp::Time::TimeDelta timeout;

public:
	Brick(Playfield &pf, const BrickType &t);
	~Brick();

	const BrickType &get_type() const { return type; }

	void set_color(const BrickColor &);
	void set_color(const Msp::GL::Color &);
	void set_bonus_type(const BonusType *bt) { bonus_type=bt; }
	const BonusType *get_bonus_type() const { return bonus_type; }
	unsigned get_durability() const { return durability; }
	void set_timeout(const Msp::Time::TimeDelta &);
	void hit(unsigned, float);

	virtual void step(const Msp::Time::TimeDelta &);
	virtual void setup_render(const Msp::GL::Tag &) const;
	virtual void finish_render(const Msp::GL::Tag &) const;
	virtual unsigned get_level_of_detail() const;
};

#endif
