/* $Id: device.cpp 20 2008-03-06 12:54:18Z tdb $

This file is part of libmspgbase
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/strings/formatter.h>
#include "device.h"

namespace Msp {
namespace Input {

bool Device::get_button_state(unsigned btn) const
{
	if(btn>buttons.size())
		return false;

	return buttons[btn];
}

float Device::get_axis_value(unsigned axis) const
{
	if(axis>axes.size())
		return 0;

	return axes[axis];
}

std::string Device::get_button_name(unsigned btn) const
{
	return format("Button %d", btn);
}

std::string Device::get_axis_name(unsigned axis) const
{
	return format("Axis %d", axis);
}

void Device::set_button_state(unsigned btn, bool state, bool event)
{
	if(btn>=buttons.size())
		buttons.resize(btn+1, false);

	if(state!=buttons[btn])
	{
		buttons[btn]=state;

		if(event)
		{
			if(state)
				signal_button_press.emit(btn);
			else
				signal_button_release.emit(btn);
		}
	}
}

void Device::set_axis_value(unsigned axis, float value, bool event)
{
	if(axis>=axes.size())
		axes.resize(axis+1, 0);

	if(value!=axes[axis])
	{
		float old=axes[axis];
		axes[axis]=value;

		if(event)
			signal_axis_motion.emit(axis, value, value-old);
	}
}

} // namespace Input
} // namespace Msp
