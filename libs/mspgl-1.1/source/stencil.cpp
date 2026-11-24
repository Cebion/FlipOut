/* $Id: stencil.cpp 28 2007-10-20 10:50:51Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "stencil.h"

namespace Msp {
namespace GL {

void stencil_func(Predicate func, int ref, uint mask)
{
	glStencilFunc(func, ref, mask);
}

void stencil_op(StencilOp sfail, StencilOp dfail, StencilOp dpass)
{
	glStencilOp(sfail, dfail, dpass);
}

} // namespace GL
} // namespace Msp
