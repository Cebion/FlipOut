/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/time/units.h>
#include "dynamicobject.h"
#include "flipout.h"
#include "playfield.h"

using namespace Msp;

DynamicObject::DynamicObject(Playfield &pf):
	Object(pf),
	body(dBodyCreate(playfield.get_world())),
	geom(0),
	reset_timeout(false),
	n_contacts(0),
	prev_contacts(0),
	contact_below(false),
	moving(false)
{
	active=true;
}

DynamicObject::~DynamicObject()
{
	if(geom)
		dGeomDestroy(geom);
	dBodyDestroy(body);
}

float DynamicObject::get_mass() const
{
	dMass mass;
	dBodyGetMass(body, &mass);
	return mass.mass;
}

void DynamicObject::set_position(const Vector3 &p)
{
	dBodySetPosition(body, p.x, p.y, p.z);
}

Vector3 DynamicObject::get_position() const
{
	const dReal *pos=dBodyGetPosition(body);
	return Vector3(pos[0], pos[1], pos[2]);
}

void DynamicObject::set_quaternion(const Quaternion &q)
{
	dQuaternion q2={ q.w, q.x, q.y, q.z };
	dBodySetQuaternion(body, q2);
}

Quaternion DynamicObject::get_quaternion() const
{
	const dReal *q=dBodyGetQuaternion(body);
	return Quaternion(q[0], q[1], q[2], q[3]);
}

void DynamicObject::set_linear_velocity(const Vector3 &v)
{
	dBodySetLinearVel(body, v.x, v.y, v.z);
}

Vector3 DynamicObject::get_linear_velocity() const
{
	const dReal *vel=dBodyGetLinearVel(body);
	return Vector3(vel[0], vel[1], vel[2]);
}

void DynamicObject::set_angular_velocity(const Vector3 &v)
{
	dBodySetAngularVel(body, v.x, v.y, v.z);
}

Vector3 DynamicObject::get_angular_velocity() const
{
	const dReal *vel=dBodyGetAngularVel(body);
	return Vector3(vel[0], vel[1], vel[2]);
}

void DynamicObject::add_force(const Vector3 &f)
{
	dBodyAddForce(body, f.x, f.y, f.z);
	reset_timeout=true;
}

void DynamicObject::activate()
{
	reset_timeout=true;
}

void DynamicObject::contact(Object *obj)
{
	++n_contacts;

	if(obj)
	{
		if(obj->is_active())
			active=true;
		if(obj->is_stale())
			reset_timeout=true;

		if(DynamicObject *dobj=dynamic_cast<DynamicObject *>(obj))
		{
			Vector3 pos=get_position();
			Vector3 other_pos=dobj->get_position();
			if(other_pos.z<pos.z)
				contact_below=true;
		}
		else
			contact_below=true;
	}
}

void DynamicObject::step(const Msp::Time::TimeDelta &dt)
{
	if(!playfield.get_flipout().get_settings().optimize_physics)
		return;

	const dReal *lv=dBodyGetLinearVel(body);
	double lspeed=lv[0]*lv[0] + lv[1]*lv[1] + lv[2]*lv[2];
	const dReal *av=dBodyGetAngularVel(body);
	double aspeed=av[0]*av[0] + av[1]*av[1] + av[2]*av[2];

	const double limit=0.25;

	moving=(lspeed>=limit || aspeed>=limit);

	if(!moving && !reset_timeout && n_contacts>=prev_contacts && contact_below)
		inactive_timeout-=dt;
	else
		inactive_timeout=400*Msp::Time::msec;

	if(inactive_timeout>Msp::Time::zero && !dBodyIsEnabled(body))
		dBodyEnable(body);
	else if(inactive_timeout<=Msp::Time::zero && dBodyIsEnabled(body))
		dBodyDisable(body);

	active=dBodyIsEnabled(body);

	reset_timeout=false;
	prev_contacts=n_contacts;
	n_contacts=0;
	contact_below=false;
}
