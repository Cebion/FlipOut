/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef COLLIDER_H_
#define COLLIDER_H_

#include <sigc++/slot.h>
#include <ode/ode.h>
#include "object.h"

class Collider
{
public:
	virtual ~Collider() { }
	virtual int collide(Object &o1, Object &o2, dContact &c) const =0;
};

template<typename T1, typename T2>
class Collider2: public Collider
{
public:
	typedef sigc::slot<bool, T1 &, T2 &, dContact &> Slot;

private:
	Slot slot;

public:
	Collider2(const Slot &s): slot(s) { }

	virtual int collide(Object &o1, Object &o2, dContact &c) const
	{
		T1 *to1=dynamic_cast<T1 *>(&o1);
		T2 *to2=dynamic_cast<T2 *>(&o2);
		if(to1 && to2)
			return slot(*to1, *to2, c);

		to1=dynamic_cast<T1 *>(&o2);
		to2=dynamic_cast<T2 *>(&o1);
		if(to1 && to2)
			return slot(*to1, *to2, c);

		return -1;
	}
};

#endif
