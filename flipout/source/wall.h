/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef WALL_H_
#define WALL_H_

#include <msp/gl/renderable.h>
#include "object.h"

class Wall: public Object, public Msp::GL::Renderable
{
public:
	enum Side
	{
		FLOOR,
		LEFT,
		RIGHT,
		TOP
	};

private:
	Side side;
	dGeomID geom;
	Msp::GL::Mesh mesh;

public:
	Wall(Playfield &, Side);
	~Wall();

	Side get_side() const { return side; }

	virtual bool has_pass(const Msp::GL::Tag &) const;
	virtual void render(const Msp::GL::Tag &) const;

private:
	void mkquad(const Vector3 *p, const int *m);
};

#endif
