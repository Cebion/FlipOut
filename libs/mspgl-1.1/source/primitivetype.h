/* $Id: primitivetype.h 38 2007-12-29 20:47:08Z tdb $

This file is part of libmspgl
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_PRIMITIVETYPE_H_
#define MSP_GL_PRIMITIVETYPE_H_

#include <istream>
#include "gl.h"

namespace Msp {
namespace GL {

enum PrimitiveType
{
	POINTS         = GL_POINTS,
	LINES          = GL_LINES,
	LINE_STRIP     = GL_LINE_STRIP,
	LINE_LOOP      = GL_LINE_LOOP,
	TRIANGLES      = GL_TRIANGLES,
	TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
	TRIANGLE_FAN   = GL_TRIANGLE_FAN,
	QUADS          = GL_QUADS,
	QUAD_STRIP     = GL_QUAD_STRIP,
	POLYGON        = GL_POLYGON
};

std::istream &operator>>(std::istream &in, PrimitiveType &pt);

} // namespace GL
} // namespace Msp

#endif
