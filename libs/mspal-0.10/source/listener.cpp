/* $Id: listener.cpp 5 2008-07-08 11:35:53Z tdb $

This file is part of libmspal
Copyright © 2008 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "listener.h"

namespace Msp {
namespace AL {

void Listener::attribute(ALenum attr, float v)
{
	alListenerf(attr, v);
}

void Listener::attribute(ALenum attr, float v0, float v1, float v2)
{
	alListener3f(attr, v0, v1, v2);
}

void Listener::attribute(ALenum attr, const float *v)
{
	alListenerfv(attr, v);
}

void Listener::set_position(float x, float y, float z)
{
	attribute(AL_POSITION, x, y, z);
}

void Listener::set_gain(float g)
{
	attribute(AL_GAIN, g);
}

} // namespace AL
} // namespace Msp
