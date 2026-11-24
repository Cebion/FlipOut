/* $Id: simplewindow.h 41 2009-08-03 12:26:54Z tdb $

This file is part of libmspgbase
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GBASE_SIMPLEWINDOW_H_
#define MSP_GBASE_SIMPLEWINDOW_H_

#include "display.h"
#include "glcontext.h"
#include "window.h"

namespace Msp {
namespace Graphics {

/**
Helper class for SimpleWindow.
*/
class SimpleWindowBase
{
protected:
	Display dpy;

	SimpleWindowBase() { }
};


/**
A simplified Window that encapsulates a Display.
*/
class SimpleWindow: public SimpleWindowBase, public Window
{
public:
	SimpleWindow(unsigned, unsigned, bool =false);
};


/**
A SimpleWindow bundled with a GLContext.
*/
class SimpleGLWindow: public SimpleWindow
{
private:
	GLContext gl_ctx;

public:
	SimpleGLWindow(unsigned, unsigned, bool =false);
	GLContext &get_gl_context() { return gl_ctx; }
	void swap_buffers();
};

} // namespace Graphics
} // namespace Msp

#endif
