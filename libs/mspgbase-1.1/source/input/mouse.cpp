/* $Id: mouse.cpp 31 2008-10-06 15:14:35Z tdb $

This file is part of libmspgbase
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/strings/formatter.h>
#include "mouse.h"

namespace Msp {
namespace Input {

Mouse::Mouse(Graphics::Window &w):
	window(w)
{
	name="Mouse";

	buttons.resize(3);
	axes.resize(2);

	window.signal_button_press.connect(sigc::mem_fun(this, &Mouse::button_press));
	window.signal_button_release.connect(sigc::mem_fun(this, &Mouse::button_release));
	window.signal_pointer_motion.connect(sigc::mem_fun(this, &Mouse::pointer_motion));
}

std::string Mouse::get_button_name(unsigned btn) const
{
	switch(btn)
	{
	case 1:
		return "Left";
	case 2:
		return "Middle";
	case 3:
		return "Right";
	case 4:
		return "Wheel Up";
	case 5:
		return "Wheel Down";
	default:
		return format("Button %d", btn);
	}
}

std::string Mouse::get_axis_name(unsigned axis) const
{
	switch(axis)
	{
	case 0:
		return "X axis";
	case 1:
		return "Y axis";
	default:
		return format("Axis %d", axis);
	};
}

void Mouse::button_press(int, int, unsigned btn, unsigned)
{
	set_button_state(btn, true, true);
}

void Mouse::button_release(int, int, unsigned btn, unsigned)
{
	set_button_state(btn, false, true);
}

void Mouse::pointer_motion(int x, int y)
{
	set_axis_value(0, x*2.0f/window.get_width()-1.0f, true);
	set_axis_value(1, 1.0f-y*2.0f/window.get_height(), true);
}

} // namespace Input
} // namespace Msp
