/* $Id: window_priv.h 30 2008-10-05 08:57:50Z tdb $

This file is part of libmspgbase
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GBASE_WINDOW_PRIV_H_
#define MSP_GBASE_WINDOW_PRIV_H_

#ifdef WIN32
#include <windows.h>
#else
#include <X11/Xlib.h>
#endif

namespace Msp {
namespace Graphics {

#ifdef WIN32
typedef HWND WindowHandle;
#else
typedef ::Window WindowHandle;
#endif

struct Window::Private
{
	WindowHandle window;
#ifndef WIN32
	Atom wm_delete_window;
	Cursor invisible_cursor;
#endif
};

struct Window::Event
{
#ifdef WIN32
	UINT msg;
	WPARAM wparam;
	LPARAM lparam;
#else
	XEvent xevent;
#endif
};

} // namespace Graphics
} // namespace Msp

#endif
