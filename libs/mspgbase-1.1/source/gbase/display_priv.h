/* $Id: display_priv.h 30 2008-10-05 08:57:50Z tdb $

This file is part of libmspgbase
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GBASE_DISPLAY_PRIV_H_
#define MSP_GBASE_DISPLAY_PRIV_H_

#include "window_priv.h"

namespace Msp {
namespace Graphics {

struct Display::Private
{
#ifndef WIN32
	::Display *display;
#endif
	std::map<WindowHandle, Window *> windows;
};

} // namespace Graphics
} // namespace Msp

#endif
