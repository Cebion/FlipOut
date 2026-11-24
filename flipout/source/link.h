/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef LINK_H_
#define LINK_H_

#include <msp/gl/renderable.h>
#include "object.h"

class Ball;

class Link: public Object, public Msp::GL::Renderable
{
private:
	struct Ray
	{
		float angle;
		std::vector<float> offsets;
	};

	Ball &ball1;
	Ball &ball2;
	float max_len;
	float strength;
	std::vector<Ray> rays;

public:
	Link(Playfield &, Ball &, Ball &, double);
	~Link();

	void destroy();

	virtual void step(const Msp::Time::TimeDelta &);
	virtual bool has_pass(const Msp::GL::Tag &) const;
	virtual void render(const Msp::GL::Tag &) const;
private:
	void resize_rays(unsigned);
};

#endif
