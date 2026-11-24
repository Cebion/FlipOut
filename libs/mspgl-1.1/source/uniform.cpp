/* $Id: uniform.cpp 30 2007-10-26 12:58:54Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "arb_shader_objects.h"
#include "uniform.h"

namespace Msp {
namespace GL {

Uniform1i::Uniform1i(int v_):
	v(v_)
{ }

void Uniform1i::apply(int index) const
{
	glUniform1iARB(index, v);
}


Uniform1f::Uniform1f(float v_):
	v(v_)
{ }

void Uniform1f::apply(int index) const
{
	glUniform1fARB(index, v);
}


Uniform2f::Uniform2f(float v0, float v1)
{
	v[0]=v0;
	v[1]=v1;
}

void Uniform2f::apply(int index) const
{
	glUniform2fvARB(index, 1, v);
}


Uniform3f::Uniform3f(float v0, float v1, float v2)
{
	v[0]=v0;
	v[1]=v1;
	v[2]=v2;
}

void Uniform3f::apply(int index) const
{
	glUniform3fvARB(index, 1, v);
}


Uniform4f::Uniform4f(float v0, float v1, float v2, float v3)
{
	v[0]=v0;
	v[1]=v1;
	v[2]=v2;
	v[3]=v3;
}

void Uniform4f::apply(int index) const
{
	glUniform4fvARB(index, 1, v);
}

} // namespace GL
} // namespace Msp
