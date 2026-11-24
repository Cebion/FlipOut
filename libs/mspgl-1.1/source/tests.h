/* $Id: tests.h 38 2007-12-29 20:47:08Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_TESTS_H_
#define MSP_GL_TESTS_H_

#include "gl.h"
#include "predicate.h"
#include "types.h"

namespace Msp {
namespace GL {

enum
{
	SCISSOR_TEST = GL_SCISSOR_TEST,
	ALPHA_TEST   = GL_ALPHA_TEST,
	DEPTH_TEST   = GL_DEPTH_TEST
};

void scissor(int left, int bottom, sizei width, sizei height);

void alpha_func(Predicate func, float ref);

void depth_func(Predicate func);

} // namespace GL
} // namespace Msp

#endif
