/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/al/source.h>
#include <msp/gl/matrix.h>
#include "ball.h"
#include "bonus.h"
#include "bonustype.h"
#include "flipout.h"
#include "geomcategory.h"
#include "misc.h"
#include "paddle.h"
#include "playfield.h"
#include "vector3.h"

using namespace Msp;

Bonus::Bonus(Playfield &pf, const BonusType &t):
	DynamicObject(pf),
	ObjectInstance(t),
	type(t)
{
	geom=dCreateCapsule(pf.get_space(), 0.3, 1);

	dMass mass;
	dMassSetCapsule(&mass, 0.7, 1, 0.3, 1);
	dBodySetMass(body, &mass);

	dGeomSetBody(geom, body);
	dQuaternion quat={ 0.7071, 0, 0.7071, 0 };
	dGeomSetOffsetQuaternion(geom, quat);
	dGeomSetCategoryBits(geom, BONUS);
	dGeomSetCollideBits(geom, FLOOR|WALL|PADDLE|BRICK|BONUS);

	dGeomSetData(geom, static_cast<Object *>(this));

	playfield.add(*this);
	playfield.get_scene().add(*this);
}

Bonus::~Bonus()
{
	playfield.remove(*this);
	playfield.get_scene().remove(*this);
}

void Bonus::collect()
{
	if(playfield.get_flipout().get_settings().sfx_enabled)
	{
		if(const AL::Buffer *snd=type.get_collect_sound())
		{
			Vector3 pos=get_position();
			AL::Source &src=playfield.get_soundscape().play(*snd, pos.x, pos.y, pos.z);
			src.set_rolloff_factor(0);
			src.set_gain(playfield.get_flipout().get_settings().sfx_volume/100.0f);
		}
	}

	type.run_actions(playfield, *this);
	stale=true;
}

void Bonus::step(const Msp::Time::TimeDelta &dt)
{
	DynamicObject::step(dt);

	const dReal *pos=dBodyGetPosition(body);
	if(pos[1]<-5)
		stale=true;

	if(dBodyIsEnabled(body))
	{
		dMass mass;
		dBodyGetMass(body, &mass);

		const dReal *avel=dBodyGetAngularVel(body);
		double f=-mass.mass*sqrt(avel[0]*avel[0] + avel[1]*avel[1] + avel[2]*avel[2])*0.005;
		dBodyAddTorque(body, avel[0]*f, avel[1]*f, avel[2]*f);

		dBodyAddForce(body, 0, -playfield.get_gravity()*mass.mass, 0);
	}
}

void Bonus::setup_render(const GL::Tag &) const
{
	GL::push_matrix();

	double matrix[16];
	get_matrix(body, matrix);
	GL::mult_matrix(matrix);
}

void Bonus::finish_render(const GL::Tag &) const
{
	GL::pop_matrix();
}
