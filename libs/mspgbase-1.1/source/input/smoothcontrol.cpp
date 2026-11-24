/* $Id: smoothcontrol.cpp 31 2008-10-06 15:14:35Z tdb $

This file is part of libmspgbase
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "smoothcontrol.h"

namespace Msp {
namespace Input {

SmoothControl::SmoothControl():
	value(0),
	paired_ctrl(0),
	dead_zone(0.1),
	threshold(0.9)
{ }

SmoothControl::SmoothControl(const ControlSource &s):
	Control(s),
	value(0),
	paired_ctrl(0),
	dead_zone(0.1),
	threshold(0.9)
{ }

SmoothControl::SmoothControl(Device &d, ControlSrcType t, unsigned i):
	Control(d, t, i),
	value(0),
	paired_ctrl(0),
	dead_zone(0.1),
	threshold(0.9)
{ }

SmoothControl::~SmoothControl()
{
	pair(0);
}

void SmoothControl::set_dead_zone(float d)
{
	dead_zone=d;
}

void SmoothControl::set_threshold(float t)
{
	threshold=t;
}

void SmoothControl::pair(SmoothControl *ctrl)
{
	if(ctrl==paired_ctrl)
		return;

	if(paired_ctrl)
	{
		SmoothControl *old_pair=paired_ctrl;
		paired_ctrl=0;
		old_pair->pair(0);
	}

	paired_ctrl=ctrl;

	if(paired_ctrl)
		paired_ctrl->pair(this);
}

void SmoothControl::on_press()
{
	on_motion(1, 1-value);
}

void SmoothControl::on_release()
{
	if(value>0)
		on_motion(0, -value);
}

void SmoothControl::on_motion(float v, float r)
{
	if(v<-threshold)
		value=-1;
	else if(v>threshold)
		value=1;
	else if(v<-dead_zone)
		value=(v+dead_zone)/(threshold-dead_zone);
	else if(v>dead_zone)
		value=(v-dead_zone)/(threshold-dead_zone);
	else
		value=0;

	signal_motion.emit(value);

	if(paired_ctrl && (v>0 || (v==0 && paired_ctrl->value!=0)))
		paired_ctrl->on_motion(-v, -r);
}

} // namespace Input
} // namespace Msp
