/* $Id: format.h 5 2008-07-08 11:35:53Z tdb $

This file is part of libmspal
Copyright © 2008 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_AL_FORMAT_H_
#define MSP_AL_FORMAT_H_

#include <AL/al.h>

namespace Msp {
namespace AL {

enum Format
{
	MONO8    = AL_FORMAT_MONO8,
	MONO16   = AL_FORMAT_MONO16,
	STEREO8  = AL_FORMAT_STEREO8,
	STEREO16 = AL_FORMAT_STEREO16
};

} // namespace AL
} // namespace Msp

#endif
