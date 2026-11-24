/* $Id: glcontext.h 29 2008-10-04 18:42:13Z tdb $

This file is part of libmspgbase
Copyright © 2007-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GBASE_GLCONTEXT_H_
#define MSP_GBASE_GLCONTEXT_H_

namespace Msp {
namespace Graphics {

class Display;
class Window;

struct GLOptions
{
	bool alpha;
	bool stencil;
	bool doublebuffer;
	unsigned multisample;

	GLOptions();
};

class GLContext
{
private:
	struct Private;

	Display &display;
	Window &window;
	Private *priv;

public:
	GLContext(Window &wnd, const GLOptions &opts=GLOptions());
	~GLContext();

	void swap_buffers();
private:
	void window_resized(unsigned, unsigned);
};

} // namespace Graphics
} // namespace Msp

#endif
