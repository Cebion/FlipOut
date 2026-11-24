/* $Id: control.h 31 2008-10-06 15:14:35Z tdb $

This file is part of libmspgbase
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GBASE_CONTROL_H_
#define MSP_GBASE_CONTROL_H_

#include <string>
#include <sigc++/signal.h>
#include <sigc++/trackable.h>

namespace Msp {
namespace Input {

class Device;

enum ControlSrcType
{
	NONE,
	BUTTON,
	AXIS_POS,
	AXIS_NEG
};

/**
Specifies the source of a control.  This provides a way for setting sources for
different types of controls in a uniform way.
*/
struct ControlSource
{
	Device *dev;
	ControlSrcType type;
	unsigned index;

	ControlSource();
	ControlSource(Device &, ControlSrcType, unsigned);
	std::string str() const;
};

/**
Provides further abstraction on top of input devices.  There are two types of
controls currently defined: BinaryControl and SmoothControl.

A control uses either a button or half of an axis (positive or negative) as its
source.  How the source values are interpreted depends on the exact type of the
control.  Controls also support interactive binding by capturing a button press
or axis motion.
*/
class Control: public sigc::trackable
{
public:
	sigc::signal<void> signal_capture_complete;

protected:
	ControlSource src;
	Device *capture_dev;

	Control();
	Control(const ControlSource &);
	Control(Device &, ControlSrcType, unsigned);
public:
	virtual ~Control() { }

	void capture(Device &);
	void cancel_capture();
	void set_source(Device &, ControlSrcType, unsigned);
	void set_source(const ControlSource &);
	const ControlSource &get_source() const { return src; }
protected:
	virtual void on_press() =0;
	virtual void on_release() =0;
	virtual void on_motion(float, float) =0;

private:
	void connect_signals();
	void button_press(unsigned);
	void button_release(unsigned);
	void axis_motion(unsigned, float, float);

	Control(const Control &);
	Control &operator=(const Control &);
};

} // namespace Input
} // namespace Msp

#endif
