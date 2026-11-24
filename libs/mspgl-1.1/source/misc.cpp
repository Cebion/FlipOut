/* $Id: misc.cpp 84 2009-09-21 13:50:58Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "misc.h"

namespace Msp {
namespace GL {

void enable(GLenum state)
{
	glEnable(state);
}

void disable(GLenum state)
{
	glDisable(state);
}

void set(GLenum state, bool value)
{
	if(value)
		enable(state);
	else
		disable(state);
}

void get(GLenum state, int &data)
{
	glGetIntegerv(state, &data);
}

void get(GLenum state, int *data)
{
	glGetIntegerv(state, data);
}

int get_i(GLenum state)
{
	int data;
	get(state, &data);
	return data;
}

} // namespace GL
} // namespace Msp
