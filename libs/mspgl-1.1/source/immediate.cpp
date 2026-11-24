/* $Id: immediate.cpp 77 2009-05-24 10:56:35Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "immediate.h"

namespace Msp {
namespace GL {

Immediate::Immediate(VertexFormat f):
	PrimitiveBuilder(array),
	array(f)
{ }

void Immediate::reset()
{
	if(in_batch)
		throw InvalidState("Can't reset Immediate between begin() and end()");

	array.clear();
	indices.clear();
}

void Immediate::end_()
{
	array.apply();
	draw_elements(type, indices.size(), &indices[0]);

	indices.clear();
}

void Immediate::element_(unsigned i)
{
	indices.push_back(i);
}

} // namespace GL
} // namespace Msp
