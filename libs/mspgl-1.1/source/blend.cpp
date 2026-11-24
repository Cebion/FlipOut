/* $Id: blend.cpp 61 2008-09-08 19:39:39Z tdb $

This file is part of libmspgl
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "blend.h"
#include "extension.h"
#include "version_1_2.h"

namespace Msp {
namespace GL {

void blend_equation(BlendEquation eq)
{
	static RequireVersion _ver(1, 2);
	glBlendEquation(eq);
}

void blend_func(BlendFactor src, BlendFactor dst)
{
	glBlendFunc(src, dst);
}

} // namespace GL
} // namespace Msp
