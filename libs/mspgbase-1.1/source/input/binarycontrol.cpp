/* $Id: binarycontrol.cpp 31 2008-10-06 15:14:35Z tdb $

This file is part of libmspgbase
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "binarycontrol.h"
#include "device.h"

namespace Msp {
namespace Input {

BinaryControl::BinaryControl():
	state(false),
	threshold(0.5)
{ }

BinaryControl::BinaryControl(const ControlSource &s):
	Control(s),
	state(false),
	threshold(0.5)
{ }

BinaryControl::BinaryControl(Device &d, ControlSrcType t, unsigned i):
	Control(d, t, i),
	state(false),
	threshold(0.5)
{ }

void BinaryControl::set_threshold(float t)
{
	threshold=t;
}

void BinaryControl::on_press()
{
	if(!state)
	{
		state=true;
		signal_press.emit();
	}
}

void BinaryControl::on_release()
{
	if(state)
	{
		state=false;
		signal_release.emit();
	}
}

void BinaryControl::on_motion(float value, float)
{
	if(value>threshold)
		on_press();
	else
		on_release();
}

} // namespace Input
} // namespace Msp
