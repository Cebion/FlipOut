/* $Id: device.h 6 2008-07-08 15:21:39Z tdb $

This file is part of libmspal
Copyright © 2008 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_AL_DEVICE_H_
#define MSP_AL_DEVICE_H_

#include <string>
#include <AL/alc.h>

namespace Msp {
namespace AL {

class Device
{
private:
	ALCdevice *dev;

public:
	Device();
	Device(const std::string &);
	~Device();

	ALCdevice *get_device() { return dev; }
};

} // namespace AL
} // namespace Msp

#endif
