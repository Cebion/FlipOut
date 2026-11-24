/* $Id: mouse.h 31 2008-10-06 15:14:35Z tdb $

This file is part of libmspgbase
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GBASE_MOUSE_H_
#define MSP_GBASE_MOUSE_H_

#include "device.h"
#include "../gbase/window.h"

namespace Msp {
namespace Input {

/**
Mouse device.  Receives events from a Graphics::Window and presents them in a
uniform way.

Note: Y axis grows upwards.
*/
class Mouse: public Device
{
private:
	Graphics::Window &window;

public:
	Mouse(Graphics::Window &);
	virtual std::string get_button_name(unsigned) const;
	virtual std::string get_axis_name(unsigned) const;
private:
	void button_press(int, int, unsigned, unsigned);
	void button_release(int, int, unsigned, unsigned);
	void pointer_motion(int, int);
};

} // namespace Input
} // namespace Msp

#endif
