/* $Id: immediate.h 77 2009-05-24 10:56:35Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_IMMEDIATE_H_
#define MSP_GL_IMMEDIATE_H_

#include "primitivebuilder.h"

namespace Msp {
namespace GL {

/**
Draws primitives on the screen.  This works similarly to the OpenGL immediate
mode: call begin() to start a batch of primitives, specify vertices, and call
end() to terminate the batch.  However, unlike OpenGL immediate mode, vertices
are not drawn as they are specified.  Instead, they are accumulated in a
VertexArray and drawn when end() is called.
*/
class Immediate: public PrimitiveBuilder
{
private:
	VertexArray array;
	std::vector<unsigned> indices;

public:
	Immediate(VertexFormat);
	void reset();
private:
	virtual void begin_() { }
	virtual void end_();
	virtual void element_(unsigned);
};

} // namespace GL
} // namespace Msp

#endif
