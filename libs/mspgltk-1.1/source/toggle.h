/* $Id: toggle.h 55 2009-09-20 16:27:08Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_TOGGLE_H_
#define MSP_GLTK_TOGGLE_H_

#include <sigc++/signal.h>
#include "text.h"
#include "widget.h"

namespace Msp {
namespace GLtk {

/**
Allows toggling a value between two states.
*/
class Toggle: public Widget
{
public:
	class Loader: public Widget::Loader
	{
	public:
		Loader(Toggle &);
		Toggle &get_object() const;
	private:
		virtual void finish();
		void text(const std::string &);
	};

private:
	Text text;
	bool pressed;
	bool value;
	bool exclusive;

public:
	sigc::signal<void, bool> signal_toggled;

	Toggle(const Resources &, const std::string & =std::string());

	void set_text(const std::string &);
	void set_exclusive(bool);
	bool get_exclusive() const { return exclusive; }
	void set_value(bool);
	bool get_value() const { return value; }

	virtual void button_press(int, int, unsigned);
	virtual void button_release(int, int, unsigned);
private:
	virtual const char *get_class() const { return "toggle"; }
	virtual void render_special(const Part &) const;

	void exclude_siblings();
};

} // namespace GLtk
} // namespace Msp

#endif
