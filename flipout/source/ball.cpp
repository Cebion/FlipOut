/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cmath>
#include <msp/gl/immediate.h>
#include <msp/gl/matrix.h>
#include <msp/gl/misc.h>
#include <msp/gl/transform.h>
#include <msp/time/units.h>
#include "ball.h"
#include "balltype.h"
#include "brick.h"
#include "bricktype.h"
#include "geomcategory.h"
#include "link.h"
#include "misc.h"
#include "paddle.h"
#include "playfield.h"
#include "vector3.h"

using namespace std;
using namespace Msp;

Ball::Ball(Playfield &pf, const BallType &t):
	DynamicObject(pf),
	GL::ObjectInstance(t),
	type(t),
	stuck(2),  // Initialize to 2 to avoid generating the stuck signal from a new ball
	link(0)
{
	geom=dCreateSphere(pf.get_space(), type.get_radius());

	dMass mass;
	dMassSetSphere(&mass, 1, type.get_radius());
	dBodySetMass(body, &mass);

	dBodySetPosition(body, 0, 0, type.get_radius());

	dGeomSetBody(geom, body);
	dGeomSetCategoryBits(geom, BALL);
	dGeomSetCollideBits(geom, FLOOR|WALL|PADDLE|BALL|BRICK|BONUS);
	dGeomSetData(geom, static_cast<Object *>(this));

	playfield.add(*this);
	playfield.get_scene().add(*this);
}

Ball::~Ball()
{
	if(link)
		link->destroy();

	playfield.remove(*this);
	playfield.get_scene().remove(*this);
}

void Ball::set_link(Link *l)
{
	link=l;
}

void Ball::step(const Msp::Time::TimeDelta &stepsize)
{
	Vector3 pos=get_position();
	Vector3 vel=get_linear_velocity();

	if(pos.y<-10)
		stale=true;

	if(double magnet=playfield.get_ball_magnet())
	{
		Vector3 paddle_pos=playfield.get_paddle()->get_position();
		double dx=paddle_pos.x-pos.x;
		double stop=vel.x*vel.x/magnet/2;
		if((vel.x>0 && dx<stop) || (vel.x<0 && dx<-stop))
			dBodyAddForce(body, -magnet, 0, 0);
		else
			dBodyAddForce(body, magnet, 0, 0);
	}

	float link_dist=type.get_link_distance();
	if(link_dist>0 && !link && pos.y>-2)
	{
		const set<Ball *> &balls=playfield.get_balls();
		for(set<Ball *>::const_iterator i=balls.begin(); i!=balls.end(); ++i)
			if(*i!=this && !(*i)->is_linked())
			{
				Vector3 pos2=(*i)->get_position();
				Vector3 delta=pos2-pos;
				if(pos2.y>-2 && dot(delta, delta)<link_dist*link_dist)
				{
					new Link(playfield, *this, **i, link_dist);
					break;
				}
			}
	}

	dMass mass;
	dBodyGetMass(body, &mass);
	dBodyAddForce(body, 0, -playfield.get_gravity()*mass.mass, 0);

	/* Stuck detection.  If the ball's speed drops below 0.35 and then doesn't
	   rise above 0.5 within 5 seconds, the ball is determined to be stuck. */
	double speed=sqrt(dot(vel, vel));
	if(!stuck && speed<0.35)
	{
		stuck=1;
		stuck_timeout=5*Time::sec;
	}
	else if(stuck)
	{
		if(speed>0.5)
		{
			if(stuck==2)
				signal_stuck.emit(false);
			stuck=0;
		}
		else if(stuck==1)
		{
			stuck_timeout-=stepsize;
			if(stuck_timeout<Time::zero)
			{
				stuck=2;
				signal_stuck.emit(true);
			}
		}
	}
}

void Ball::setup_render(const GL::Tag &) const
{
	GL::push_matrix();

	double matrix[16];
	get_matrix(body, matrix);
	GL::mult_matrix(matrix);
}

void Ball::finish_render(const GL::Tag &) const
{
	GL::pop_matrix();
}
