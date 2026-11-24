/* $Id: dropdown.cpp 55 2009-09-20 16:27:08Z tdb $

This file is part of libmspgltk
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/font.h>
#include "dropdown.h"
#include "list.h"
#include "panel.h"
#include "part.h"
#include "style.h"
#include "text.h"

using namespace std;

namespace Msp {
namespace GLtk {

Dropdown::Dropdown(const Resources &r):
	Widget(r),
	Container(r),
	list(r),
	dropped(false)
{
	add(list);
	list.signal_item_selected.connect(sigc::mem_fun(this, &Dropdown::list_item_selected));

	update_style();
}

void Dropdown::append(const string &item)
{
	list.append(item);
	resize_list();
}

void Dropdown::insert(unsigned i, const string &v)
{
	list.insert(i, v);
	resize_list();
}

void Dropdown::remove(unsigned i)
{
	list.remove(i);
	resize_list();
}

void Dropdown::clear()
{
	list.clear();
	resize_list();
}

unsigned Dropdown::get_n_items() const
{
	return list.get_n_items();
}

void Dropdown::set_selected_index(int i)
{
	list.set_selected_index(i);
}

const string &Dropdown::get_selected() const
{
	return list.get_selected();
}

int Dropdown::get_selected_index() const
{
	return list.get_selected_index();
}

void Dropdown::button_press(int x, int y, unsigned btn)
{
	if(dropped)
	{
		Container::button_press(x, y, btn);
		if(!click_focus)
		{
			dropped=false;
			state&=~ACTIVE;
			signal_ungrab_pointer.emit();
		}
	}
	else if(btn==1)
	{
		dropped=true;
		state|=ACTIVE;
		signal_grab_pointer.emit();
	}
}

void Dropdown::render_special(const Part &part) const
{
	if(part.get_name()=="text")
	{
		if(list.get_selected_index()>=0)
			Text(style, list.get_selected()).render(part, geom);
			//render_text(part, list.get_selected());
	}
	else if(part.get_name()=="list" && dropped)
		list.render();
}

void Dropdown::on_geometry_change()
{
	resize_list();
}

void Dropdown::resize_list()
{
	// XXX This is a hack.
	unsigned n_items=list.get_n_items();
	const Style &stl=list.get_style();
	const GL::Font &font=*stl.get_font();
	unsigned h=min(max(n_items, 1U), 10U)*static_cast<unsigned>((font.get_ascent()-font.get_descent())*font.get_default_size());
	for(std::list<Part>::const_iterator i=stl.get_parts().begin(); i!=stl.get_parts().end(); ++i)
		if(i->get_name()=="items")
		{
			const Sides &margin=i->get_margin();
			h+=margin.top+margin.bottom;
		}
	list.set_geometry(Geometry(0, -h, geom.w, h));
}

void Dropdown::list_item_selected(unsigned index, const std::string &item)
{
	if(dropped)
	{
		dropped=false;
		state&=~ACTIVE;
		signal_ungrab_pointer.emit();
	}

	signal_item_selected.emit(index, item);
}


Dropdown::Loader::Loader(Dropdown &d):
	Widget::Loader(d)
{
	add("item", &Loader::item);
}

void Dropdown::Loader::item(const string &str)
{
	dynamic_cast<Dropdown &>(wdg).append(str);
}

} // namespace GLtk
} // namespace Msp
