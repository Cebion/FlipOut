/* $Id: panel.cpp 54 2009-09-16 19:16:18Z tdb $

This file is part of libmspgltk
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <algorithm>
#include <msp/core/refptr.h>
#include "button.h"
#include "dropdown.h"
#include "entry.h"
#include "hslider.h"
#include "indicator.h"
#include "label.h"
#include "list.h"
#include "panel.h"
#include "part.h"
#include "table.h"
#include "toggle.h"
#include "vslider.h"

using namespace std;

namespace Msp {
namespace GLtk {

Panel::Panel(const Resources &r):
	Widget(r),
	Container(r),
	pointer_focus(0),
	pointer_grabbed(false),
	input_focus(0)
{
	update_style();
}

void Panel::raise(Widget &wdg)
{
	for(list<Container::Child *>::iterator i=children.begin(); i!=children.end(); ++i)
		if((*i)->widget==&wdg)
		{
			children.splice(children.end(), children, i);
			return;
		}

	throw InvalidState("That Widget is not in this Panel");
}

void Panel::button_press(int x, int y, unsigned btn)
{
	if(pointer_grabbed)
	{
		const Geometry &cgeom=pointer_focus->get_geometry();
		pointer_focus->button_press(x-cgeom.x, y-cgeom.y, btn);
	}
	else
	{
		if(Widget *wdg=get_child_at(x, y))
		{
			set_pointer_focus(wdg);
			set_input_focus(wdg);
		}
		Container::button_press(x, y, btn);
	}
}

void Panel::button_release(int x, int y, unsigned btn)
{
	if(pointer_grabbed)
	{
		const Geometry &cgeom=pointer_focus->get_geometry();
		pointer_focus->button_release(x-cgeom.x, y-cgeom.y, btn);
	}
	else
		Container::button_release(x, y, btn);
}

void Panel::pointer_motion(int x, int y)
{
	if(pointer_grabbed)
	{
		const Geometry &cgeom=pointer_focus->get_geometry();
		pointer_focus->pointer_motion(x-cgeom.x, y-cgeom.y);
	}
	else
	{
		set_pointer_focus(get_child_at(x, y));
		Container::pointer_motion(x, y);
	}
}

void Panel::pointer_leave()
{
	Container::pointer_leave();
	set_pointer_focus(0);
}

void Panel::key_press(unsigned key, unsigned mod, wchar_t ch)
{
	if(input_focus)
		input_focus->key_press(key, mod, ch);
}

void Panel::key_release(unsigned key, unsigned mod)
{
	if(input_focus)
		input_focus->key_release(key, mod);
}

void Panel::focus_out()
{
	set_input_focus(0);
}

void Panel::render_special(const Part &part) const
{
	if(part.get_name()=="children")
	{
		for(list<Container::Child *>::const_iterator i=children.begin(); i!=children.end(); ++i)
			if((*i)->widget->is_visible())
				(*i)->widget->render();
	}
}

Panel::Child *Panel::create_child(Widget *wdg)
{
	return new Child(*this, wdg);
}

void Panel::set_pointer_focus(Widget *wdg)
{
	if(wdg!=pointer_focus)
	{
		if(pointer_focus)
			pointer_focus->pointer_leave();

		pointer_focus=wdg;

		if(pointer_focus)
			pointer_focus->pointer_enter();
	}
}

void Panel::set_input_focus(Widget *wdg)
{
	if(wdg!=input_focus)
	{
		if(input_focus)
			input_focus->focus_out();

		input_focus=wdg;

		if(input_focus)
		{
			raise(*wdg);
			input_focus->focus_in();
		}
	}
}


Panel::Loader::Loader(Panel &p, map<string, Widget *> &m):
	Widget::Loader(p),
	pnl(p),
	wdg_map(m)
{
	add("button",    &Loader::child<Button>);
	add("dropdown",  &Loader::child<Dropdown>);
	add("entry",     &Loader::child<Entry>);
	add("hslider",   &Loader::child<HSlider>);
	add("indicator", &Loader::child<Indicator>);
	add("label",     &Loader::child<Label>);
	add("list",      &Loader::child<List>);
	add("panel",     &Loader::panel);
	add("table",     &Loader::child<Table>);
	add("toggle",    &Loader::child<Toggle>);
	add("vslider",   &Loader::child<VSlider>);
}

template<typename T>
void Panel::Loader::child(const string &n)
{
	RefPtr<T> chl=new T(pnl.res);
	load_sub(*chl);
	pnl.add(*chl.get());
	wdg_map[n]=chl.release();
}

void Panel::Loader::panel(const string &n)
{
	RefPtr<Panel> p=new Panel(pnl.res);
	load_sub(*p, wdg_map);
	pnl.add(*p.get());
	wdg_map[n]=p.release();
}


Panel::Child::Child(Panel &p, Widget *w):
	Container::Child(p, w)
{
	widget->signal_visibility_changed.connect(sigc::mem_fun(this, &Child::visibility_changed));
	widget->signal_request_focus.connect(sigc::mem_fun(this, &Child::request_focus));
	widget->signal_grab_pointer.connect(sigc::mem_fun(this, &Child::grab_pointer));
	widget->signal_ungrab_pointer.connect(sigc::mem_fun(this, &Child::ungrab_pointer));
}

Panel::Child::~Child()
{
	visibility_changed(false);
}

void Panel::Child::visibility_changed(bool v)
{
	if(!v)
	{
		Panel &panel=static_cast<Panel &>(container);
		if(widget==panel.pointer_focus)
			panel.set_pointer_focus(0);
		if(widget==panel.input_focus)
			panel.set_input_focus(0);
	}
}

void Panel::Child::request_focus()
{
	static_cast<Panel &>(container).set_input_focus(widget);
}

void Panel::Child::grab_pointer()
{
	Panel &panel=static_cast<Panel &>(container);
	if(!panel.pointer_grabbed)
	{
		panel.set_pointer_focus(widget);
		panel.pointer_grabbed=true;
		panel.signal_grab_pointer.emit();
	}
}

void Panel::Child::ungrab_pointer()
{
	Panel &panel=static_cast<Panel &>(container);
	if(panel.pointer_grabbed && panel.pointer_focus==widget)
	{
		// XXX Should set to the widget under pointer
		panel.set_pointer_focus(0);
		panel.pointer_grabbed=false;
		panel.signal_ungrab_pointer.emit();
	}
}

} // namespace GLtk
} // namespace Msp
