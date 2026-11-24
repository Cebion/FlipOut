/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef OBJECT_H_
#define OBJECT_H_

#include <ode/ode.h>
#include <msp/time/timedelta.h>

class Playfield;

class Object
{
protected:
	Playfield &playfield;
	bool active;
	bool stale;

	Object(Playfield &);
public:
	virtual ~Object() { }

	bool is_active() const { return active; }
	bool is_stale() const { return stale; }

	virtual void contact(Object *) { }
	virtual void step(const Msp::Time::TimeDelta &) { }
};

#endif
