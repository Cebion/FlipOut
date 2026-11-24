/* $Id: binarycontrol.h 31 2008-10-06 15:14:35Z tdb $

This file is part of libmspgbase
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GBASE_BINARYCONTROL_H_
#define MSP_GBASE_BINARYCONTROL_H_

#include "control.h"

namespace Msp {
namespace Input {

/**
A control with two possible states.  Button state is mapped directly.  An axis
is considered to be active when its value is within 10% of the end of the axis.
*/
class BinaryControl: public Control
{
public:
	sigc::signal<void> signal_press;
	sigc::signal<void> signal_release;

private:
	bool state;
	float threshold;

public:
	BinaryControl();
	BinaryControl(const ControlSource &);
	BinaryControl(Device &, ControlSrcType, unsigned);

	/**
	Sets the threshold between states for axis sources.  No effect on button
	sources
	*/
	void set_threshold(float);

	bool get_state() const { return state; }

private:
	virtual void on_press();
	virtual void on_release();
	virtual void on_motion(float, float);
};

} // namespace Input
} // namespace Msp

#endif
