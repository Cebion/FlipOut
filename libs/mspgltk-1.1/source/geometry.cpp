/* $Id: geometry.cpp 46 2008-10-11 15:09:51Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "geometry.h"

namespace Msp {
namespace GLtk {

bool Geometry::is_inside(int x_, int y_) const
{
	return (x_>=x && x_<x+static_cast<int>(w) && y_>=y && y_<y+static_cast<int>(h));
}

bool Geometry::is_inside_relative(int x_, int y_) const
{
	return (x_>=0 && x_<static_cast<int>(w) && y_>=0 && y_<static_cast<int>(h));
}


Sides::Sides():
	top(0),
	right(0),
	bottom(0),
	left(0)
{ }


Sides::Loader::Loader(Sides &s):
	sides(s)
{
	add("top",    &Sides::top);
	add("right",  &Sides::right);
	add("bottom", &Sides::bottom);
	add("left",   &Sides::left);
}


void Alignment::apply(Geometry &geom, const Geometry &parent) const
{
	if(parent.w>geom.w)
	{
		geom.w+=static_cast<unsigned>((parent.w-geom.w)*w);
		geom.x+=static_cast<int>((parent.w-geom.w)*x);
	}
	if(parent.h>geom.h)
	{
		geom.h+=static_cast<unsigned>((parent.h-geom.h)*h);
		geom.y+=static_cast<int>((parent.h-geom.h)*y);
	}
}

void Alignment::apply(Geometry &geom, const Geometry &parent, const Sides &margin) const
{
	unsigned pw=parent.w-margin.left-margin.right;
	unsigned ph=parent.h-margin.bottom-margin.top;

	geom.x+=margin.left;
	geom.y+=margin.bottom;

	if(pw>geom.w)
	{
		geom.w+=static_cast<unsigned>((pw-geom.w)*w);
		geom.x+=static_cast<int>((pw-geom.w)*x);
	}
	if(ph>geom.h)
	{
		geom.h+=static_cast<unsigned>((ph-geom.h)*h);
		geom.y+=static_cast<int>((ph-geom.h)*y);
	}
}

} // namespace GLtk
} // namespace Msp
