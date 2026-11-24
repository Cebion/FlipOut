/* $Id: device.cpp 6 2008-07-08 15:21:39Z tdb $

This file is part of libmspal
Copyright © 2008 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/core/except.h>
#include "device.h"

using namespace std;

namespace Msp {
namespace AL {

Device::Device()
{
	dev=alcOpenDevice(0);
	if(!dev)
		throw Exception("Couldn't get OpenAL device");
}

Device::Device(const string &spec)
{
	dev=alcOpenDevice(spec.c_str());
	if(!dev)
		throw Exception("Couldn't get OpenAL device");
}

Device::~Device()
{
	alcCloseDevice(dev);
}

} // namespace AL
} // namespace Msp
