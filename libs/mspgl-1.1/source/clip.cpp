/* $Id: clip.cpp 52 2008-07-19 11:19:01Z tdb $

This file is part of libmspgl
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "clip.h"
#include "gl.h"

namespace Msp {
namespace GL {

ClipPlane::ClipPlane(double a, double b, double c, double d)
{
	eq[0]=a;
	eq[1]=b;
	eq[2]=c;
	eq[3]=d;
}

void ClipPlane::apply_to(unsigned n)
{
	glClipPlane(GL_CLIP_PLANE0+n, eq);
	glEnable(GL_CLIP_PLANE0+n);
}

void ClipPlane::disable(unsigned n)
{
	glDisable(GL_CLIP_PLANE0+n);
}

} // namespace GL
} // namespace Msp
