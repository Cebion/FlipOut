/* $Id: smoothcontrol.h 31 2008-10-06 15:14:35Z tdb $

This file is part of libmspgbase
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GBASE_SMOOTHCONTROL_H_
#define MSP_GBASE_SMOOTHCONTROL_H_

#include "control.h"

namespace Msp {
namespace Input {

/**
A control with a continuous range of values.  A button is treated as either 1
or 0.  Axis values are mapped to absolute value.

Two smooth controls can also be paired to each other.  Motion on one control of
the pair will cause negative motion on the other.  This works best when the
controls are bound to the opposite sides of the same axis.
*/
class SmoothControl: public Control
{
public:
	sigc::signal<void, float> signal_motion;

private:
	float value;
	SmoothControl *paired_ctrl;
	float dead_zone;
	float threshold;

public:
	SmoothControl();
	SmoothControl(const ControlSource &);
	SmoothControl(Device &, ControlSrcType, unsigned);
	virtual ~SmoothControl();

	/// Sets the dead zone value.  Any value below this will be treated as 0.
	void set_dead_zone(float);

	/// Sets the max-out threshold.  Any value above this will be treated as 1.
	void set_threshold(float);

	void pair(SmoothControl *ctrl);
	float get_value() const { return value; }

private:
	virtual void on_press();
	virtual void on_release();
	virtual void on_motion(float, float);
};

} // namespace Input
} // namespace Msp

#endif
