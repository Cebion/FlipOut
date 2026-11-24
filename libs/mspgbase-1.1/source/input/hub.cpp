/* $Id: hub.cpp 31 2008-10-06 15:14:35Z tdb $

This file is part of libmspgbase
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <sigc++/bind.h>
#include <msp/core/except.h>
#include "hub.h"

namespace Msp {
namespace Input {

Hub::Hub()
{
	name="Hub";
}

unsigned Hub::attach(Device &dev)
{
	unsigned index=devices.size();
	devices.push_back(&dev);
	dev.signal_button_press.connect(sigc::bind(sigc::mem_fun(this, &Hub::button_press), index));
	dev.signal_button_release.connect(sigc::bind(sigc::mem_fun(this, &Hub::button_release), index));
	dev.signal_axis_motion.connect(sigc::bind(sigc::mem_fun(this, &Hub::axis_motion), index));
	return index;
}

std::string Hub::get_button_name(unsigned btn) const
{
	unsigned dev_index=btn>>12;
	if(dev_index>=devices.size())
		throw InvalidParameterValue("Button does not exist");

	const Device &dev=*devices[dev_index];
	return dev.get_name()+": "+dev.get_button_name(btn&0xFFF);
}

std::string Hub::get_axis_name(unsigned axis) const
{
	unsigned dev_index=axis>>12;
	if(dev_index>=devices.size())
		throw InvalidParameterValue("Axis does not exist");

	const Device &dev=*devices[dev_index];
	return dev.get_name()+": "+dev.get_axis_name(axis&0xFFF);
}

void Hub::button_press(unsigned btn, unsigned index)
{
	set_button_state((index<<12) | (btn&0xFFF), true, true);
}

void Hub::button_release(unsigned btn, unsigned index)
{
	set_button_state((index<<12) | (btn&0xFFF), false, true);
}

void Hub::axis_motion(unsigned axis, float value, float, unsigned index)
{
	set_axis_value((index<<12) | (axis&0xFFF), value, true);
}

} // namespace Input
} // namespace Msp
