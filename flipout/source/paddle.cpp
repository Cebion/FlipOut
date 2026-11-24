/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/matrix.h>
#include <msp/gl/transform.h>
#include <msp/time/units.h>
#include "ball.h"
#include "balltype.h"
#include "controls.h"
#include "flipout.h"
#include "geomcategory.h"
#include "misc.h"
#include "paddle.h"
#include "paddletype.h"
#include "playfield.h"

using namespace std;
using namespace Msp;

// XXX Figure out how to make the prismatic-and-rotoide joint work
Paddle::Paddle(Playfield &pf, const PaddleType &t):
	Object(pf),
	ObjectInstance(t),
	type(t),
	slider_body(dBodyCreate(pf.get_world())),
	slider(dJointCreateSlider(pf.get_world(), 0)),
	lmotor(dJointCreateLMotor(pf.get_world(), 0)),
	body(dBodyCreate(pf.get_world())),
	hinge(dJointCreateHinge(pf.get_world(), 0)),
	amotor(dJointCreateAMotor(pf.get_world(), 0)),
	angle(0),
	speed(0),
	turbo(1),
	attached_ball(0)
{
	dMass mass;
	dMassSetBox(&mass, 1, 0.5, 0.5, 0.5);
	dBodySetMass(slider_body, &mass);
	dBodySetGravityMode(slider_body, 0);

	dMassSetBox(&mass, 1, type.get_width(), type.get_height(), type.get_depth());
	dBodySetMass(body, &mass);

	dBodySetGravityMode(body, 0);

	dBodySetPosition(slider_body, 0, 1, type.get_depth()/2);
	dBodySetPosition(body, 0, 1, type.get_depth()/2);

	dJointAttach(slider, slider_body, 0);
	dJointSetSliderAxis(slider, 1, 0, 0);
	dJointSetSliderParam(slider, dParamLoStop, -(pf.get_width()-type.get_width())/2);
	dJointSetSliderParam(slider, dParamHiStop, (pf.get_width()-type.get_width())/2);

	dJointSetLMotorParam(lmotor, dParamFMax, playfield.get_flipout().get_settings().sensitivity*100);
	dJointSetLMotorNumAxes(lmotor, 1);
	dJointSetLMotorAxis(lmotor, 0, 0, 1, 0, 0);
	dJointAttach(lmotor, slider_body, 0);

	dJointAttach(hinge, slider_body, body);
	dJointSetHingeAxis(hinge, 0, 0, -1);

	dJointSetAMotorParam(amotor, dParamFMax, 1000);
	dJointSetAMotorNumAxes(amotor, 1);
	dJointSetAMotorAxis(amotor, 0, 0, 0, 0, 1);
	dJointAttach(amotor, body, 0);

	float w=type.get_width();
	if(type.get_round_ends())
		w-=type.get_height();

	geoms.push_back(dCreateBox(pf.get_space(), w, type.get_height(), type.get_depth()));
	if(type.get_round_ends())
	{
		geoms.push_back(dCreateCapsule(pf.get_space(), type.get_height()/2, type.get_depth()));
		geoms.push_back(dCreateCapsule(pf.get_space(), type.get_height()/2, type.get_depth()));
	}

	for(unsigned i=0; i<geoms.size(); ++i)
	{
		dGeomSetBody(geoms[i], body);
		dGeomSetCategoryBits(geoms[i], PADDLE);
		dGeomSetCollideBits(geoms[i], BALL|BONUS);
		if(i>=1)
			dGeomSetOffsetPosition(geoms[i], (type.get_width()-type.get_height())*(i==1 ? 0.5 : -0.5), 0, 0);
		dGeomSetData(geoms[i], static_cast<Object *>(this));
	}

	material.set_ambient(GL::Color(0.8));

	Controls &ctrls=pf.get_flipout().get_controls();
	ctrls.move_right.signal_motion.connect(sigc::mem_fun(this, &Paddle::control_motion));
	ctrls.flip_right.signal_motion.connect(sigc::mem_fun(this, &Paddle::flip));
	ctrls.turbo.signal_press.connect(sigc::mem_fun(this, &Paddle::turbo_on));
	ctrls.turbo.signal_release.connect(sigc::mem_fun(this, &Paddle::turbo_off));

	playfield.add(*this);
	playfield.get_scene().add(*this);
}

Paddle::~Paddle()
{
	for(vector<dGeomID>::iterator i=geoms.begin(); i!=geoms.end(); ++i)
		dGeomDestroy(*i);
	dJointDestroy(slider);
	dJointDestroy(lmotor);
	dJointDestroy(hinge);
	dJointDestroy(amotor);
	dBodyDestroy(slider_body);
	dBodyDestroy(body);

	playfield.remove(*this);
	playfield.get_scene().remove(*this);
}

float Paddle::get_width() const
{
	return type.get_width();
}

float Paddle::get_height() const
{
	return type.get_height();
}

Vector3 Paddle::get_position() const
{
	const dReal *pos=dBodyGetPosition(body);
	return Vector3(pos[0], pos[1], pos[2]);
}

Vector3 Paddle::get_normal() const
{
	const dReal *rot=dBodyGetRotation(body);
	return Vector3(rot[1], rot[5], rot[9]);
}

void Paddle::attach(Ball &b)
{
	attached_ball=&b;
	place_attached_ball();
}

void Paddle::step(const Msp::Time::TimeDelta &)
{
	if(attached_ball)
		place_attached_ball();

	dReal x=dBodyGetPosition(body)[0];
	dJointSetLMotorParam(lmotor, dParamVel, min(max(target-x, -2.0), 2.0)*speed*turbo);
	dJointSetAMotorParam(amotor, dParamVel, (angle-dJointGetHingeAngle(hinge))*30);
}

void Paddle::setup_render(const GL::Tag &) const
{
	GL::push_matrix();
	double matrix[16];
	get_matrix(body, matrix);
	GL::mult_matrix(matrix);

	material.bind();
}

void Paddle::finish_render(const GL::Tag &) const
{
	GL::Material::unbind();
	GL::pop_matrix();
}

void Paddle::control_motion(float value)
{
	if(value)
	{
		target=value*(playfield.get_width()-type.get_width())/2;
		speed=playfield.get_flipout().get_settings().speed*2;
	}
	else
	{
		// Special case for keyboard control - stop when key is released
		target=dBodyGetPosition(body)[0];
		speed=0;
	}
}

void Paddle::flip(float v)
{
	if(v)
		attached_ball=0;
	angle=v*0.3;
}

void Paddle::turbo_on()
{
	turbo=1.7;
}

void Paddle::turbo_off()
{
	turbo=1;
}

void Paddle::place_attached_ball()
{
	float r=attached_ball->get_type().get_radius();
	Vector3 p=get_position();
	p.y+=type.get_height()/2+r;
	p.z=r;
	attached_ball->set_position(p);
	attached_ball->set_linear_velocity(Vector3());
	attached_ball->set_angular_velocity(Vector3());
}
