/* $Id: drawcontext.h 30 2008-10-05 08:57:50Z tdb $

This file is part of libmspgbase
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GBASE_DRAWCONTEXT_H_
#define MSP_GBASE_DRAWCONTEXT_H_

namespace Msp {
namespace Graphics {

class Display;
class Window;

class DrawContext
{
private:
	struct Private;

	Display &display;
	Window &window;
	Private *priv;

public:
	DrawContext(Window &);
	~DrawContext();

	Window &get_window() const { return window; }
	unsigned get_depth() const;
	unsigned char *get_data();
	void update();
};

} // namespace Graphics
} // namespace Msp

#endif
