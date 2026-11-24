/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef PADDLE_H_
#define PADDLE_H_

#include <vector>
#include <ode/ode.h>
#include <sigc++/trackable.h>
#include <msp/gl/material.h>
#include <msp/gl/objectinstance.h>
#include "object.h"

class Ball;
class PaddleType;
class Playfield;
class Vector3;

class Paddle: public Object, public Msp::GL::ObjectInstance, public sigc::trackable
{
private:
	const PaddleType &type;

	dBodyID slider_body;
	dJointID slider;
	dJointID lmotor;

	dBodyID body;
	dJointID hinge;
	dJointID amotor;

	std::vector<dGeomID> geoms;

	double target;
	double angle;
	double speed;
	double turbo;
	Ball *attached_ball;

	Msp::GL::Material material;

public:
	Paddle(Playfield &, const PaddleType &);
	~Paddle();

	float get_width() const;
	float get_height() const;
	Vector3 get_position() const;
	Vector3 get_normal() const;
	void attach(Ball &);

	virtual void step(const Msp::Time::TimeDelta &);
	virtual void setup_render(const Msp::GL::Tag &) const;
	virtual void finish_render(const Msp::GL::Tag &) const;

private:
	void control_motion(float);
	void flip(float);
	void turbo_on();
	void turbo_off();
	void place_attached_ball();
};

#endif
