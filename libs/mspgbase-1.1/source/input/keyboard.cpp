/* $Id: keyboard.cpp 30 2008-10-05 08:57:50Z tdb $

This file is part of libmspgbase
Copyright © 2007-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifdef WIN32
#include <windows.h>
#else
#include <X11/Xlib.h>
#endif
#include <msp/strings/formatter.h>
#include "../gbase/display.h"
#include "keyboard.h"
#include "keys.h"

#define MAPVK_VK_TO_VSC 0

namespace Msp {
namespace Input {

Keyboard::Keyboard(Graphics::Window &w):
	window(w)
{
	name="Keyboard";

	buttons.resize(N_KEYS_, false);

	window.signal_key_press.connect(sigc::mem_fun(this, &Keyboard::key_press));
	window.signal_key_release.connect(sigc::mem_fun(this, &Keyboard::key_release));
}

std::string Keyboard::get_button_name(unsigned btn) const
{
	if(btn==0)
		return "None";
#ifndef WIN32
	const char *str=XKeysymToString(key_to_sys(btn));
	if(!str)
		return format("Key %d", btn);
	return str;
#else
	char buf[128];
	unsigned scan=MapVirtualKey(key_to_sys(btn), MAPVK_VK_TO_VSC);
	if(!GetKeyNameText(scan<<16, buf, sizeof(buf)))
		return format("Key %d", btn);
	return buf;
#endif
}

void Keyboard::key_press(unsigned key, unsigned, unsigned)
{
	set_button_state(key_from_sys(key), true, true);
}

void Keyboard::key_release(unsigned key, unsigned)
{
	set_button_state(key_from_sys(key), false, true);
}

} // namespace Input
} // namespace Msp
