/* $Id: control.cpp 31 2008-10-06 15:14:35Z tdb $

This file is part of libmspgbase
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/core/except.h>
#include <msp/strings/lexicalcast.h>
#include "control.h"
#include "device.h"

namespace Msp {
namespace Input {

ControlSource::ControlSource():
	dev(0),
	type(NONE),
	index(0)
{ }

ControlSource::ControlSource(Device &d, ControlSrcType t, unsigned i):
	dev(&d),
	type(t),
	index(i)
{ }

std::string ControlSource::str() const
{
	if(type==BUTTON)
		return dev->get_button_name(index);
	else if(type==AXIS_POS)
		return dev->get_axis_name(index)+" +";
	else if(type==AXIS_NEG)
		return dev->get_axis_name(index)+" -";
	else if(type==NONE)
		return "None";

	return lexical_cast(index);
}


Control::Control():
	capture_dev(0)
{ }

Control::Control(const ControlSource &s):
	src(s),
	capture_dev(0)
{ }

Control::Control(Device &d, ControlSrcType t, unsigned i):
	src(d, t, i),
	capture_dev(0)
{
	connect_signals();
}

void Control::capture(Device &d)
{
	notify_callbacks();
	capture_dev=&d;
	capture_dev->signal_button_press.connect(sigc::mem_fun(this, &Control::button_press));
	capture_dev->signal_axis_motion.connect(sigc::mem_fun(this, &Control::axis_motion));
}

void Control::cancel_capture()
{
	notify_callbacks();
	capture_dev=0;
	connect_signals();
}

void Control::set_source(Device &d, ControlSrcType t, unsigned i)
{
	set_source(ControlSource(d, t, i));
}

void Control::set_source(const ControlSource &s)
{
	notify_callbacks();
	src=s;
	connect_signals();
}

void Control::connect_signals()
{
	switch(src.type)
	{
	case NONE:
		break;
	case BUTTON:
		src.dev->signal_button_press.connect(sigc::mem_fun(this, &Control::button_press));
		src.dev->signal_button_release.connect(sigc::mem_fun(this, &Control::button_release));
		break;
	case AXIS_POS:
	case AXIS_NEG:
		src.dev->signal_axis_motion.connect(sigc::mem_fun(this, &Control::axis_motion));
		break;
	default:
		throw Exception("Invalid source in Control");
	}
}

void Control::button_press(unsigned i)
{
	if(capture_dev)
	{
		src.dev=capture_dev;
		src.type=BUTTON;
		src.index=i;

		notify_callbacks();
		capture_dev=0;
		connect_signals();
		signal_capture_complete.emit();
	}
	else if(src.type==BUTTON && i==src.index)
		on_press();
}

void Control::button_release(unsigned i)
{
	if(src.type==BUTTON && i==src.index)
		on_release();
}

void Control::axis_motion(unsigned i, float v, float r)
{
	if(capture_dev)
	{
		ControlSrcType type=NONE;
		if(v<-0.9)
			type=AXIS_NEG;
		else if(v>0.9)
			type=AXIS_POS;

		if(type!=NONE)
		{
			src.dev=capture_dev;
			src.type=type;
			src.index=i;

			notify_callbacks();
			capture_dev=0;
			connect_signals();
			signal_capture_complete.emit();
		}
	}
	else if(src.type==AXIS_POS && i==src.index && v>=0)
		on_motion(v, r);
	else if(src.type==AXIS_NEG && i==src.index && v<=0)
		on_motion(-v, -r);
}

} // namespace Input
} // namespace Msp
