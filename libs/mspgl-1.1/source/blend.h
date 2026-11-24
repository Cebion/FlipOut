/* $Id: blend.h 53 2008-07-19 11:24:42Z tdb $

This file is part of libmspgl
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_BLEND_H_
#define MSP_GL_BLEND_H_

#include "gl.h"

namespace Msp {
namespace GL {

enum
{
	BLEND = GL_BLEND
};

enum BlendEquation
{
	ADD              = GL_FUNC_ADD,
	SUBTRACT         = GL_FUNC_SUBTRACT,
	REVERSE_SUBTRACT = GL_FUNC_REVERSE_SUBTRACT,
	MIN              = GL_MIN,
	MAX              = GL_MAX
};

enum BlendFactor
{
	ZERO = GL_ZERO,
	ONE = GL_ONE,
	SRC_COLOR = GL_SRC_COLOR,
	ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
	SRC_ALPHA = GL_SRC_ALPHA,
	ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
	DST_COLOR = GL_DST_COLOR,
	ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
	DST_ALPHA = GL_DST_ALPHA,
	ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
	CONSTANT_COLOR = GL_CONSTANT_COLOR,
	ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
	CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
	ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA
};

void blend_equation(BlendEquation eq);
void blend_func(BlendFactor src, BlendFactor dst);

} // namespace GL
} // namespace Msp

#endif
