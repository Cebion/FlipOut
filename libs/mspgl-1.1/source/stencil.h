/* $Id: stencil.h 38 2007-12-29 20:47:08Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_STENCIL_H_
#define MSP_GL_STENCIL_H_

#include "gl.h"
#include "predicate.h"
#include "types.h"

namespace Msp {
namespace GL {

enum
{
	STENCIL_TEST = GL_STENCIL_TEST
};

enum StencilOp
{
	KEEP      = GL_KEEP,
	SET_ZERO  = GL_ZERO,
	REPLACE   = GL_REPLACE,
	INCR      = GL_INCR,
	DECR      = GL_DECR,
	INVERT    = GL_INVERT,
	INCR_WRAP = GL_INCR_WRAP,
	DECR_WRAP = GL_DECR_WRAP
};

void stencil_func(Predicate func, int ref, uint mask);
void stencil_op(StencilOp sfail, StencilOp dfail, StencilOp dpass);

} // namespace GL
} // namespace Msp

#endif
