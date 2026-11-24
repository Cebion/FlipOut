/* $Id: root.h 55 2009-09-20 16:27:08Z tdb $

This file is part of libmspgltk
Copyright © 2007, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_ROOT_H_
#define MSP_GLTK_ROOT_H_

#include <sigc++/trackable.h>
#include <msp/gbase/window.h>
#include <msp/time/timestamp.h>
#include "panel.h"

namespace Msp {
namespace GLtk {

class Label;

/**
A Root is a special type of Panel that covers and entire Window and accepts
input from it.  When created, a Root widget will take its size from the window
it is created for.  The size can be changed, but a Root should always be
rendered to fill the window in order to get coordinates mapped correctly.
*/
class Root: public Panel, public sigc::trackable
{
private:
	Graphics::Window &window;
	Label *lbl_tooltip;
	int pointer_x;
	int pointer_y;
	Msp::Time::TimeStamp tooltip_timeout;
	Widget *tooltip_target;

public:
	Root(const Resources &, Graphics::Window &);
	void tick();
private:
	virtual const char *get_class() const { return "root"; }
	void button_press_event(int, int, unsigned, unsigned);
	void button_release_event(int, int, unsigned, unsigned);
	void pointer_motion_event(int, int);
	void key_press_event(unsigned, unsigned, wchar_t);
	void key_release_event(unsigned, unsigned);
	void translate_coords(int &, int &);
};

} // namespace GLtk
} // namespace Msp

#endif
