/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef BALL_H_
#define BALL_H_

#include <sigc++/signal.h>
#include <ode/ode.h>
#include <msp/gl/objectinstance.h>
#include "dynamicobject.h"

class BallType;
class Link;
class Playfield;
class Vector3;

/**
Balls roll around the playfield and break bricks.  They collide with
everything.
*/
class Ball: public DynamicObject, public Msp::GL::ObjectInstance
{
public:
	/**
	Emitted when the ball's stuck status changes.  The parameter is the new
	status.
	*/
	sigc::signal<void, bool> signal_stuck;

private:
	const BallType &type;
	int stuck;
	Msp::Time::TimeDelta stuck_timeout;
	Link *link;

public:
	Ball(Playfield &pf, const BallType &);
	~Ball();

	const BallType &get_type() const { return type; }
	bool is_stuck() const { return stuck==2; }
	void set_link(Link *);
	bool is_linked() const { return link; }

	virtual void step(const Msp::Time::TimeDelta &);
	virtual void setup_render(const Msp::GL::Tag &) const;
	virtual void finish_render(const Msp::GL::Tag &) const;
};

#endif
