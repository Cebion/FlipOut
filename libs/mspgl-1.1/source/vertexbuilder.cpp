/* $Id: vertexbuilder.cpp 72 2009-01-10 10:07:36Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "vertexbuilder.h"

namespace Msp {
namespace GL {

VertexBuilder::VertexBuilder():
	cr(1), cg(1), cb(1), ca(1),
	ts(0), tt(0), tr(0), tq(0),
	nx(0), ny(0), nz(1)
{ }

void VertexBuilder::attrib(unsigned i, float x, float y, float z, float w)
{
	Attrib &a=av[i];
	a.x=x;
	a.y=y;
	a.z=z;
	a.w=w;
}

} // namespace GL
} // namespace Msp
