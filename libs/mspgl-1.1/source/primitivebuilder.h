/* $Id: primitivebuilder.h 60 2008-09-05 22:36:05Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_PRIMITIVEBUILDER_H_
#define MSP_GL_PRIMITIVEBUILDER_H_

#include "primitivetype.h"
#include "vertexarray.h"
#include "vertexbuilder.h"

namespace Msp {
namespace GL {

class VertexArray;
class VertexArrayBuilder;

/**
Base class for primitive builders.  This is derived from VertexBuilder and adds
begin() and end() functions for specifying batches of primitives instead of
just vertices.
*/
class PrimitiveBuilder: public VertexBuilder
{
protected:
	VertexArray &array;
	VertexArrayBuilder vab;
	PrimitiveType type;
	bool in_batch;

	PrimitiveBuilder(VertexArray &);
public:
	void begin(PrimitiveType);
	void end();
	void element(unsigned);
	PrimitiveType get_type() const;
protected:
	virtual void vertex_(float, float, float, float);
	virtual void begin_() =0;
	virtual void end_() =0;
	virtual void element_(unsigned) =0;
};

} // namespace GL
} // namespace Msp

#endif
