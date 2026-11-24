/* $Id: listener.h 8 2008-08-19 09:41:12Z tdb $

This file is part of libmspal
Copyright © 2008 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_AL_LIStENER_H_
#define MSP_AL_LISTENER_H_

#include <AL/al.h>

namespace Msp {
namespace AL {

/**
Represents the listener in the 3D environment.  There is only one listener in
OpenAL - this class is not instantiable and all of its functions are static.
*/
class Listener
{
private:
	Listener();
public:
	static void attribute(ALenum, float);
	static void attribute(ALenum, float, float, float);
	static void attribute(ALenum, const float *);
	static void set_position(float, float, float);
	static void set_gain(float);
};

} // namespace AL
} // namespace Msp

#endif
