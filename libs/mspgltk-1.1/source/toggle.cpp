/* $Id: toggle.cpp 55 2009-09-20 16:27:08Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "panel.h"
#include "part.h"
#include "toggle.h"

using namespace std;

namespace Msp {
namespace GLtk {

Toggle::Toggle(const Resources &r, const string &t):
	Widget(r),
	text(style),
	pressed(false),
	value(false),
	exclusive(false)
{
	update_style();
	set_text(t);
}

void Toggle::set_text(const string &t)
{
	text=t;
}

void Toggle::set_exclusive(bool e)
{
	exclusive=e;
	if(exclusive && value)
		exclude_siblings();
}

void Toggle::set_value(bool v)
{
	value=v;
	if(value)
	{
		state|=ACTIVE;
		if(exclusive && parent)
			exclude_siblings();
	}
	else
		state&=~ACTIVE;
}

void Toggle::button_press(int, int, unsigned btn)
{
	if(btn==1)
		pressed=true;
}

void Toggle::button_release(int x, int y, unsigned btn)
{
	if(pressed && btn==1)
	{
		if(geom.is_inside_relative(x, y) && (!value || !exclusive))
		{
			set_value(!value);
			signal_toggled.emit(value);
		}

		pressed=false;
	}
}

void Toggle::render_special(const Part &part) const
{
	if(part.get_name()=="text")
		text.render(part, geom);
}

void Toggle::exclude_siblings()
{
	const list<Widget *> &siblings=parent->get_children();
	for(list<Widget *>::const_iterator i=siblings.begin(); i!=siblings.end(); ++i)
		if(Toggle *tgl=dynamic_cast<Toggle *>(*i))
			if(tgl!=this && tgl->get_exclusive() && tgl->get_value())
				tgl->set_value(false);
}


Toggle::Loader::Loader(Toggle &t):
	Widget::Loader(t)
{
	add("exclusive", &Toggle::exclusive);
	add("text",      &Loader::text);
	add("value",     &Toggle::value);
}

Toggle &Toggle::Loader::get_object() const
{
	return static_cast<Toggle &>(wdg);
}

void Toggle::Loader::finish()
{
	Toggle &tgl=static_cast<Toggle &>(wdg);
	if(tgl.value)
		tgl.state|=ACTIVE;
	else
		tgl.state&=~ACTIVE;
}

void Toggle::Loader::text(const string &t)
{
	static_cast<Toggle &>(wdg).text=t;
}

} // namespace GLtk
} // namespace Msp
