/* $Id: context.cpp 6 2008-07-08 15:21:39Z tdb $

This file is part of libmspal
Copyright © 2008 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/core/except.h>
#include "context.h"
#include "device.h"

namespace Msp {
namespace AL {

Context::Context(Device &dev)
{
	context=alcCreateContext(dev.get_device(), 0);
	if(!context)
		throw Exception("Couldn't create OpenAL context");

	alcMakeContextCurrent(context);
}

Context::~Context()
{
	alcDestroyContext(context);
}

} // namespace AL
} // namespace Msp
