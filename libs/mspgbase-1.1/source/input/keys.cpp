/* $Id: keys.cpp 23 2008-03-19 11:42:50Z tdb $

This file is part of libmspgbase
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <map>
#ifdef WIN32
#include <windows.h>
#else
#include <X11/keysym.h>
#endif
#include <msp/core/except.h>
#include "keys.h"

using namespace std;

namespace {

unsigned keymap[Msp::Input::N_KEYS_]=
{
#ifndef WIN32
	0, 0, 0, 0, 0, 0, 0, 0,
	XK_BackSpace, XK_Tab, XK_Return, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, XK_Escape, 0, 0, 0, 0,

	XK_space, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, XK_plus, XK_comma, XK_minus, XK_period, XK_slash,
	XK_0, XK_1, XK_2, XK_3, XK_4, XK_5, XK_6, XK_7,
	XK_8, XK_9, 0, XK_semicolon, XK_less, XK_equal, 0, 0,

	0, XK_a, XK_b, XK_c, XK_d, XK_e, XK_f, XK_g,
	XK_h, XK_i, XK_j, XK_k, XK_l, XK_m, XK_n, XK_o,
	XK_p, XK_q, XK_r, XK_s, XK_t, XK_u, XK_v, XK_w,
	XK_x, XK_y, XK_z, XK_bracketleft, XK_backslash, XK_bracketright, 0, 0,

	XK_grave, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,

	XK_adiaeresis, XK_odiaeresis, XK_udiaeresis, XK_aring, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,

	XK_Left, XK_Right, XK_Up, XK_Down, 0, 0, 0, 0,
	XK_Home, XK_End, XK_Page_Up, XK_Page_Down, XK_Insert, XK_Delete, 0, 0,
	XK_F1, XK_F2, XK_F3, XK_F4, XK_F5, XK_F6, XK_F7, XK_F8,
	XK_F9, XK_F10, XK_F11, XK_F12, 0, 0, 0, 0,

	XK_Shift_L, XK_Shift_R, XK_Control_L, XK_Control_R, XK_Alt_L, XK_Alt_R, XK_Super_L, XK_Super_R,
	XK_Caps_Lock, XK_Scroll_Lock, XK_Num_Lock, 0, 0, 0, 0, 0,
	XK_KP_0, XK_KP_1, XK_KP_2, XK_KP_3, XK_KP_4, XK_KP_5, XK_KP_6, XK_KP_7,
	XK_KP_8, XK_KP_9, XK_KP_Add, XK_KP_Subtract, XK_KP_Multiply, XK_KP_Divide, XK_KP_Separator, 0,

	XK_Pause, XK_Print, XK_Menu, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
#else
	0, 0, 0, 0, 0, 0, 0, 0,
	VK_BACK, VK_TAB, VK_RETURN, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, VK_ESCAPE, 0, 0, 0, 0,

	VK_SPACE, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 0, 0, 0, 0, 0, 0,

	0, 'A', 'B', 'C', 'D', 'E', 'F', 'G',
	'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
	'X', 'Y', 'Z', 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,

	VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, 0, 0, 0, 0,
	VK_HOME, VK_END, VK_PRIOR, VK_NEXT, VK_INSERT, VK_DELETE, 0, 0,
	0, VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7,
	VK_F8, VK_F9, VK_F10, VK_F11, VK_F12, 0, 0, 0,

	VK_LSHIFT, VK_RSHIFT, VK_LCONTROL, VK_RCONTROL, 0, 0, VK_LWIN, VK_RWIN,
	VK_CAPITAL, VK_SCROLL, VK_NUMLOCK, 0, 0, 0, 0, 0,
	VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD4, VK_NUMPAD5, VK_NUMPAD6, VK_NUMPAD7,
	VK_NUMPAD8, VK_NUMPAD9, VK_ADD, VK_SUBTRACT, VK_MULTIPLY, VK_DIVIDE, VK_SEPARATOR, 0,

	VK_PAUSE, VK_SNAPSHOT, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
#endif
};

}

namespace Msp {
namespace Input {

unsigned key_from_sys(unsigned code)
{
	static bool init_done=false;
	static map<unsigned, unsigned> reverse_map;

	if(!init_done)
	{
		for(unsigned i=0; i<N_KEYS_; ++i)
			if(keymap[i])
				reverse_map[keymap[i]]=i;

		init_done=true;
	}

	map<unsigned, unsigned>::const_iterator i=reverse_map.find(code);
	if(i!=reverse_map.end())
		return i->second;

	return 0;
}

unsigned key_to_sys(unsigned key)
{
	if(key>N_KEYS_)
		throw InvalidParameterValue("Key out of range");
	return keymap[key];
}

} // namespace Input
} // namespace Msp
