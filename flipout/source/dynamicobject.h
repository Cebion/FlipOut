/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef DYNAMICOBJECT_H_
#define DYNAMICOBJECT_H_

#include <ode/ode.h>
#include "object.h"
#include "quaternion.h"
#include "vector3.h"

class DynamicObject: public Object
{
protected:
	dBodyID body;
	dGeomID geom;
	bool reset_timeout;
	Msp::Time::TimeDelta inactive_timeout;
	unsigned n_contacts;
	unsigned prev_contacts;
	bool contact_below;
	bool moving;

public:
	DynamicObject(Playfield &);
	~DynamicObject();

	float get_mass() const;
	void set_position(const Vector3 &);
	Vector3 get_position() const;
	void set_quaternion(const Quaternion &);
	Quaternion get_quaternion() const;
	void set_linear_velocity(const Vector3 &);
	Vector3 get_linear_velocity() const;
	void set_angular_velocity(const Vector3 &);
	Vector3 get_angular_velocity() const;
	void add_force(const Vector3 &);
	void activate();

	virtual void contact(Object *);
	virtual void step(const Msp::Time::TimeDelta &);
};

#endif
