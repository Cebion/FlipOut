/* $Id: primitivebuilder.cpp 60 2008-09-05 22:36:05Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "primitivebuilder.h"

namespace Msp {
namespace GL {

PrimitiveBuilder::PrimitiveBuilder(VertexArray &a):
	array(a),
	vab(array),
	in_batch(false)
{ }

void PrimitiveBuilder::begin(PrimitiveType t)
{
	if(in_batch)
		throw InvalidState("begin() already called");

	type=t;
	in_batch=true;

	begin_();
}

void PrimitiveBuilder::end()
{
	if(!in_batch)
		throw InvalidState("end() called without begin()");

	in_batch=false;

	end_();
}

void PrimitiveBuilder::element(unsigned i)
{
	if(!in_batch)
		throw InvalidState("Element specification not between begin() and end()");
	if(i>=array.size())
		throw InvalidParameterValue("Element index out of range");
	element_(i);
}

PrimitiveType PrimitiveBuilder::get_type() const
{
	if(!in_batch)
		throw InvalidState("Not between begin() and end()");
	return type;
}

void PrimitiveBuilder::vertex_(float x, float y, float z, float w)
{
	vab.texcoord(ts, tt, tr,tq);
	vab.color(cr, cg, cb, ca);
	vab.normal(nx, ny, nz);
	vab.vertex(x, y, z, w);

	if(in_batch)
		element_(array.size()-1);
}

} // namespace GL
} // namespace Msp
