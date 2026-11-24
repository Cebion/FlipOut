/* $Id: context.h 6 2008-07-08 15:21:39Z tdb $

This file is part of libmspal
Copyright © 2008 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_AL_CONTEXT_H_
#define MSP_AL_CONTEXT_H_

#include <AL/alc.h>

namespace Msp {
namespace AL {

class Device;

class Context
{
private:
	ALCcontext *context;

public:
	Context(Device &);
	~Context();
};

} // namespace AL
} // namespace Msp

#endif
