/* $Id: keyboard.h 22 2008-03-10 15:18:28Z tdb $

This file is part of libmspgbase
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GBASE_KEYBOARD_H_
#define MSP_GBASE_KEYBOARD_H_

#include "device.h"
#include "../gbase/window.h"

namespace Msp {
namespace Input {

/**
Adapts key events from a window to the abstracted input framework.  Key codes
are translated to platform-independent values.  See keys.h for a list.
*/
class Keyboard: public Device
{
private:
	Graphics::Window &window;

public:
	Keyboard(Graphics::Window &);

	virtual std::string get_button_name(unsigned) const;
private:
	void key_press(unsigned, unsigned, unsigned);
	void key_release(unsigned, unsigned);
};

} // namespace Input
} // namespace Msp

#endif
