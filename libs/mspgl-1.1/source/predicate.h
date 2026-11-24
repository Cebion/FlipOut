/* $Id: predicate.h 38 2007-12-29 20:47:08Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_PREDICATE_H_
#define MSP_GL_PREDICATE_H_

#include "gl.h"

namespace Msp {
namespace GL {

enum Predicate
{
	NEVER    = GL_NEVER,
	ALWAYS   = GL_ALWAYS,
	LESS     = GL_LESS,
	LEQUAL   = GL_LEQUAL,
	EQUAL    = GL_EQUAL,
	GREATER  = GL_GREATER,
	GEQUAL   = GL_GEQUAL,
	NOTEQUAL = GL_NOTEQUAL
};

} // namespace GL
} // namespace Msp

#endif
