/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef BONUS_H_
#define BONUS_H_

#include <ode/ode.h>
#include <msp/gl/material.h>
#include <msp/gl/objectinstance.h>
#include "dynamicobject.h"

class BonusType;
class Playfield;
class Vector3;

class Bonus: public DynamicObject, public Msp::GL::ObjectInstance
{
private:
	const BonusType &type;

public:
	Bonus(Playfield &pf, const BonusType &);
	~Bonus();

	const BonusType &get_type() const { return type; }

	void collect();

	virtual void step(const Msp::Time::TimeDelta &);
	virtual void setup_render(const Msp::GL::Tag &) const;
	virtual void finish_render(const Msp::GL::Tag &) const;
};

#endif
