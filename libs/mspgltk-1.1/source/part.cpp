/* $Id: part.cpp 27 2008-03-03 17:48:55Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/transform.h>
#include "geometry.h"
#include "part.h"
#include "resources.h"

using namespace std;

namespace Msp {
namespace GLtk {

Part::Part(const string &n):
	name(n)
{
	for(unsigned i=0; i<N_STATES_; ++i)
		graphic[i]=0;
}

const Graphic *Part::get_graphic(State state) const
{
	if(state>N_STATES_)
		throw InvalidParameterValue("Invalid state");

	return graphic[state];
}

void Part::render(const Geometry &parent, State state) const
{
	if(!graphic[state])
		return;

	Geometry rgeom=geom;
	align.apply(rgeom, parent, margin);
	GL::translate(rgeom.x, rgeom.y, 0);
	graphic[state]->render(rgeom.w, rgeom.h);
}


Part::Loader::Loader(Part &p, Resources &r):
	part(p),
	res(r)
{
	add("graphic", &Loader::graphic);
	add("align",   &Loader::align);
	add("fill",    &Loader::fill);
	add("margin",  &Loader::margin);
	add("size",    &Loader::size);
}

Part::Loader::~Loader()
{
	for(unsigned i=0; i<N_STATES_; ++i)
		if(part.graphic[i])
		{
			const Sides &shadow=part.graphic[i]->get_shadow();
			part.geom.w=max(part.geom.w, part.graphic[i]->get_width()-shadow.left-shadow.right);
			part.geom.h=max(part.geom.h, part.graphic[i]->get_height()-shadow.bottom-shadow.top);
		}
}

void Part::Loader::graphic(State s, const string &n)
{
	Graphic *grph=res.get<Graphic>(n);
	for(int i=0; i<N_STATES_; ++i)
		if((i&s)==s)
			part.graphic[i]=grph;
}

void Part::Loader::align(float x, float y)
{
	part.align.x=x;
	part.align.y=y;
}

void Part::Loader::fill(float w, float h)
{
	part.align.w=w;
	part.align.h=h;
}

void Part::Loader::margin()
{
	load_sub(part.margin);
}

void Part::Loader::size(unsigned w, unsigned h)
{
	part.geom.w=w;
	part.geom.h=h;
}

} // namespace GLtk
} // namespace Msp
