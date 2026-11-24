/* $Id: device.h 31 2008-10-06 15:14:35Z tdb $

This file is part of libmspgbase
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GBASE_INPUTDEVICE_H_
#define MSP_GBASE_INPUTDEVICE_H_

#include <string>
#include <vector>
#include <sigc++/signal.h>

namespace Msp {
namespace Input {

/**
Base class for input devices.  Input devices have two types of controls:
buttons and axes.  Buttons are either on or off.  Axes have a floating point
value in the range [-1, 1].
*/
class Device
{
public:
	sigc::signal<void, unsigned> signal_button_press;
	sigc::signal<void, unsigned> signal_button_release;
	sigc::signal<void, unsigned, float, float> signal_axis_motion;

protected:
	std::string name;
	std::vector<char>  buttons;
	std::vector<float> axes;

	Device() { }
public:
	virtual ~Device() { }
	const std::string &get_name() const { return name; }
	bool  get_button_state(unsigned) const;
	float get_axis_value(unsigned) const;

	virtual std::string get_button_name(unsigned) const;
	virtual std::string get_axis_name(unsigned) const;
protected:
	void set_button_state(unsigned, bool, bool);
	void set_axis_value(unsigned, float, bool);
};

} // namespace Input
} // namespace Msp

#endif
