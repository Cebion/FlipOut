/* $Id$

This file is part of libmspgltk
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "container.h"

using namespace std;

namespace Msp {
namespace GLtk {

Container::Container(const Resources &r):
	Widget(r),
	click_focus(0),
	click_button(0)
{ }

Container::~Container()
{
	while(!children.empty())
		delete children.front()->widget;
}

void Container::add(Widget &wdg)
{
	set_parent(wdg, this);
	children.push_back(create_child(&wdg));
}

void Container::remove(Widget &wdg)
{
	for(list<Child *>::iterator i=children.begin(); i!=children.end(); ++i)
		if((*i)->widget==&wdg)
		{
			set_parent(wdg, 0);
			delete *i;
			children.erase(i);
			return;
		}

	throw InvalidState("That Widget is not in this Container");
}

list<Widget *> Container::get_children() const
{
	list<Widget *> result;
	for(list<Child *>::const_iterator i=children.begin(); i!=children.end(); ++i)
		result.push_back((*i)->widget);
	return result;
}

Widget *Container::get_child_at(int x, int y)
{
	for(list<Child *>::iterator i=children.end(); i!=children.begin();)
		if((*--i)->widget->is_visible() && (*i)->widget->get_geometry().is_inside(x, y))
			return (*i)->widget;

	return 0;
}

Widget *Container::get_descendant_at(int x, int y)
{
	Widget *wdg=get_child_at(x, y);
	if(Container *cont=dynamic_cast<Container *>(wdg))
	{
		const Geometry &cgeom=wdg->get_geometry();
		return cont->get_descendant_at(x-cgeom.x, y-cgeom.y);
	}
	return wdg;
}

void Container::button_press(int x, int y, unsigned btn)
{
	if(click_focus)
	{
		const Geometry &cgeom=click_focus->get_geometry();
		click_focus->button_press(x-cgeom.x, y-cgeom.y, btn);
	}
	else
	{
		if(Widget *wdg=get_child_at(x, y))
		{
			click_focus=wdg;
			click_button=btn;

			const Geometry &cgeom=wdg->get_geometry();
			wdg->button_press(x-cgeom.x, y-cgeom.y, btn);
		}
	}
}

void Container::button_release(int x, int y, unsigned btn)
{
	if(click_focus)
	{
		Widget *wdg=click_focus;

		if(btn==click_button)
			click_focus=0;

		const Geometry &cgeom=wdg->get_geometry();
		wdg->button_release(x-cgeom.x, y-cgeom.y, btn);
	}
	else
	{
		if(Widget *wdg=get_child_at(x, y))
		{
			const Geometry &cgeom=wdg->get_geometry();
			wdg->button_release(x-cgeom.x, y-cgeom.y, btn);
		}
	}
}

void Container::pointer_motion(int x, int y)
{
	if(click_focus)
	{
		const Geometry &cgeom=click_focus->get_geometry();
		click_focus->pointer_motion(x-cgeom.x, y-cgeom.y);
	}
	else
	{
		Widget *wdg=get_child_at(x, y);
		if(wdg)
		{
			const Geometry &cgeom=wdg->get_geometry();
			wdg->pointer_motion(x-cgeom.x, y-cgeom.y);
		}
	}
}

void Container::pointer_leave()
{
	Widget::pointer_leave();
	click_focus=0;
}

Container::Child *Container::create_child(Widget *wdg)
{
	return new Child(*this, wdg);
}


Container::Child::Child(Container &c, Widget *w):
	container(c),
	widget(w)
{
	widget->signal_visibility_changed.connect(sigc::mem_fun(this, &Child::visibility_changed));
}

Container::Child::~Child()
{
	if(widget==container.click_focus)
		container.click_focus=0;
}

void Container::Child::visibility_changed(bool v)
{
	if(!v && widget==container.click_focus)
		container.click_focus=0;
}

} // namespace GLtk
} // namespace Msp
