/* $Id: tests.cpp 28 2007-10-20 10:50:51Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "tests.h"

namespace Msp {
namespace GL {

void scissor(int left, int bottom, sizei width, sizei height)
{
	glScissor(left, bottom, width, height);
}

void alpha_func(Predicate func, float ref)
{
	glAlphaFunc(func, ref);
}

void depth_func(Predicate func)
{
	glDepthFunc(func);
}

} // namespace GL
} // namespace Msp
