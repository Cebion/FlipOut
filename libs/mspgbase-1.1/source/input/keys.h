/* $Id: keys.h 22 2008-03-10 15:18:28Z tdb $

This file is part of libmspgbase
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_INPUT_KEYS_H_
#define MSP_INPUT_KEYS_H_

namespace Msp {
namespace Input {

enum
{
	// Codes 0x01-0x7E reserved for ASCII
	KEY_BACKSPACE = 0x08,
	KEY_TAB = 0x09,
	KEY_ENTER = 0x0A,
	KEY_ESC = 0x1B,
	KEY_SPACE = 0x20,

	KEY_APOSTROPHE = 0x27,
	KEY_PLUS = 0x2B,
	KEY_COMMA = 0x2C,
	KEY_MINUS = 0x2D,
	KEY_PERIOD = 0x2E,
	KEY_SLASH = 0x2F,

	KEY_0 = 0x30,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,

	KEY_SEMICOLON = 0x3B,
	KEY_LESS = 0x3C,
	KEY_EQUAL = 0x3D,

	KEY_A = 0x41,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,

	KEY_BRACKET_L = 0x5B,
	KEY_BACKSLASH = 0x5C,
	KEY_BRACKET_R = 0x5D,
	KEY_GRAVE = 0x60,

	// Various non-ASCII characters
	KEY_ADIAERESIS = 0x80,
	KEY_ODIAERESIS,
	KEY_UDIAERESIS,
	KEY_ARING,

	// Arrow keys
	KEY_LEFT = 0xA0,
	KEY_RIGHT,
	KEY_UP,
	KEY_DOWN,

	// The block above arrow keys
	KEY_HOME = 0xA8,
	KEY_END,
	KEY_PGUP,
	KEY_PGDN,
	KEY_INSERT,
	KEY_DELETE,

	// Function keys
	KEY_F1 = 0xB1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,

	// Modifier keys
	KEY_SHIFT_L = 0xC0,
	KEY_SHIFT_R,
	KEY_CTRL_L,
	KEY_CTRL_R,
	KEY_ALT_L,
	KEY_ALT_R,
	KEY_SUPER_L,
	KEY_SUPER_R,
	KEY_CAPS_LOCK,
	KEY_SCROLL_LOCK,
	KEY_NUM_LOCK,

	// Keypad
	KEY_KP0 = 0xD0,
	KEY_KP1,
	KEY_KP2,
	KEY_KP3,
	KEY_KP4,
	KEY_KP5,
	KEY_KP6,
	KEY_KP7,
	KEY_KP8,
	KEY_KP9,
	KEY_KP_ADD,
	KEY_KP_SUBTRACT,
	KEY_KP_MULTIPLY,
	KEY_KP_DIVIDE,
	KEY_KP_SEPARATOR,

	// Miscellaneous keys
	KEY_PAUSE = 0xE0,
	KEY_PRINT_SCREEN,
	KEY_MENU,

	N_KEYS_ = 0x100
};

extern unsigned key_from_sys(unsigned);
extern unsigned key_to_sys(unsigned);

} // namespace Input
} // namespace Msp

#endif
