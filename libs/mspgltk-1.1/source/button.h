/* $Id: button.h 55 2009-09-20 16:27:08Z tdb $

This file is part of libmspgltk
Copyright © 2007, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_BUTTON_H_
#define MSP_GLTK_BUTTON_H_

#include <sigc++/sigc++.h>
#include <msp/gl/texture2d.h>
#include "text.h"
#include "widget.h"

namespace Msp {
namespace GLtk {

/**
A clickable button widget.  Buttons can have a text label, which is displayed
in a spacial part "text".
*/
class Button: public Widget
{
public:
	class Loader: public Widget::Loader
	{
	public:
		Loader(Button &);
	private:
		void text(const std::string &);
	};

private:
	Text text;
	const GL::Texture2D *icon;
	bool pressed;

public:
	sigc::signal<void> signal_clicked;

	Button(const Resources &, const std::string & =std::string());
	void set_text(const std::string &);
	void set_icon(const GL::Texture2D *);
	virtual void button_press(int, int, unsigned);
	virtual void button_release(int, int, unsigned);
	virtual void pointer_motion(int, int);
private:
	virtual const char *get_class() const { return "button"; }
	virtual void render_special(const Part &) const;
};

} // namespace GLtk
} // namespace Msp

#endif
