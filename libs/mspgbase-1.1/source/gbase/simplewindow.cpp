/* $Id: simplewindow.cpp 41 2009-08-03 12:26:54Z tdb $

This file is part of libmspgbase
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "simplewindow.h"

namespace Msp {
namespace Graphics {

SimpleWindow::SimpleWindow(unsigned w, unsigned h, bool fs):
	Window(dpy, w, h, fs)
{ }


SimpleGLWindow::SimpleGLWindow(unsigned w, unsigned h, bool fs):
	SimpleWindow(w, h, fs),
	gl_ctx(*this)
{ }

void SimpleGLWindow::swap_buffers()
{
	gl_ctx.swap_buffers();
}

} // namespace Graphics
} // namespace Msp
