/* $Id: hub.h 31 2008-10-06 15:14:35Z tdb $

This file is part of libmspgbase
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GBASE_INPUTHUB_H_
#define MSP_GBASE_INPUTHUB_H_

#include "device.h"

namespace Msp {
namespace Input {

/**
The Hub device collects events from multiple input devices and presents an
aggregate of them.  Button and axis numbers are mapped to unique values.
*/
class Hub: public Device
{
protected:
	std::vector<Device *> devices;

public:
	Hub();

	/**
	Attaches an input device to the hub.

	@param   dev  Device to attach

	@return  Index of the device within the hub
	*/
	unsigned attach(Device &dev);

	virtual std::string get_button_name(unsigned) const;
	virtual std::string get_axis_name(unsigned) const;
protected:
	void button_press(unsigned, unsigned);
	void button_release(unsigned, unsigned);
	void axis_motion(unsigned, float, float, unsigned);
};

} // namespace Input
} // namespace Msp

#endif
