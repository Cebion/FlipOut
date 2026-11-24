/* $Id: list.cpp 54 2009-09-16 19:16:18Z tdb $

This file is part of libmspgltk
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/immediate.h>
#include <msp/gl/matrix.h>
#include <msp/gl/transform.h>
#include "graphic.h"
#include "list.h"
#include "part.h"
#include "style.h"
#include "vslider.h"

using namespace std;

namespace Msp {
namespace GLtk {

List::List(const Resources &r):
	Widget(r),
	Container(r),
	sel_index(-1),
	first(0),
	n_visible(1),
	row_height(1),
	items_part(0),
	slider(res)
{
	add(slider);
	slider.set_step(1);
	slider.signal_value_changed.connect(sigc::mem_fun(this, &List::slider_value_changed));

	update_style();
}

void List::append(const string &v)
{
	items.push_back(v);
	recalculate_parameters();
}

void List::insert(unsigned i, const string &v)
{
	if(i>items.size())
		throw InvalidParameterValue("Index out of range");

	items.insert(items.begin()+i, v);
	recalculate_parameters();
}

void List::remove(unsigned i)
{
	if(i>items.size())
		throw InvalidParameterValue("Index out of range");

	items.erase(items.begin()+i);
	if(sel_index>static_cast<int>(i))
		--sel_index;
	else if(sel_index==static_cast<int>(i))
		sel_index=-1;

	recalculate_parameters();
}

void List::clear()
{
	items.clear();
	sel_index=-1;

	recalculate_parameters();
}

void List::set_selected_index(int i)
{
	if(i<0)
		sel_index=-1;
	else if(i<static_cast<int>(items.size()))
	{
		sel_index=i;
		signal_item_selected.emit(sel_index, items[sel_index]);
	}
	else
		throw InvalidParameterValue("Index out of range");
}

const string &List::get_selected() const
{
	if(sel_index<0)
		throw InvalidState("No selection");

	return items[sel_index];
}

void List::button_press(int x, int y, unsigned btn)
{
	Container::button_press(x, y, btn);
	if(!click_focus && btn==1)
	{
		if(items_part)
			y+=items_part->get_margin().top;

		unsigned i=(geom.h-1-y)/row_height;
		if(i<n_visible && first+i<items.size())
		{
			sel_index=first+i;

			signal_item_selected.emit(sel_index, items[sel_index]);
		}
	}
}

void List::render_special(const Part &part) const
{
	if(part.get_name()=="items")
	{
		const GL::Font &font=*style->get_font();
		const float font_size=font.get_default_size();
		const GL::Color &color=style->get_font_color();
		const Sides &margin=part.get_margin();

		Geometry pgeom=geom;
		pgeom.h=row_height;
		pgeom.w-=margin.left+margin.right;

		for(unsigned i=0; (i<n_visible && first+i<items.size()); ++i)
		{
			Geometry rgeom;
			rgeom.w=static_cast<unsigned>(font.get_string_width(items[first+i])*font_size);
			rgeom.h=row_height;
			rgeom.x=margin.left;
			rgeom.y=geom.h-margin.top-(i+1)*row_height-static_cast<int>(font.get_descent()*font_size);
			part.get_alignment().apply(rgeom, pgeom);

			GL::push_matrix();
			GL::translate(rgeom.x, rgeom.y, 0);
			GL::scale_uniform(font_size);
			GL::Immediate imm((GL::COLOR4_UBYTE, GL::TEXCOORD2, GL::VERTEX2));
			imm.color(color.r, color.g, color.b);
			font.draw_string(items[first+i], imm);
			GL::pop_matrix();
		}
	}
	else if(part.get_name()=="selection")
	{
		if(sel_index>=static_cast<int>(first) && sel_index<static_cast<int>(first+n_visible))
		{
			const Sides &margin=part.get_margin();

			Geometry pgeom=geom;
			pgeom.h=row_height;
			pgeom.w-=margin.left+margin.right;

			Geometry rgeom=part.get_geometry();
			rgeom.y+=geom.h-margin.top-row_height*(sel_index-first+1);
			rgeom.x+=margin.left;
			part.get_alignment().apply(rgeom, pgeom);

			GL::push_matrix();
			GL::translate(rgeom.x, rgeom.y, 0);
			part.get_graphic(state)->render(rgeom.w, rgeom.h);
			GL::pop_matrix();
		}
	}
	else if(part.get_name()=="slider")
		slider.render();
}

void List::on_geometry_change()
{
	reposition_slider();

	recalculate_parameters();
}

void List::on_style_change()
{
	reposition_slider();

	items_part=0;
	for(list<Part>::const_iterator i=style->get_parts().begin(); i!=style->get_parts().end(); ++i)
		if(i->get_name()=="items")
			items_part=&*i;

	const GL::Font &font=*style->get_font();
	row_height=static_cast<unsigned>((font.get_ascent()-font.get_descent())*font.get_default_size());

	recalculate_parameters();
}

void List::reposition_slider()
{
	for(list<Part>::const_iterator i=style->get_parts().begin(); i!=style->get_parts().end(); ++i)
		if(i->get_name()=="slider")
		{
			Geometry sgeom=i->get_geometry();
			i->get_alignment().apply(sgeom, geom, i->get_margin());
			slider.set_geometry(sgeom);
		}
}

void List::recalculate_parameters()
{
	unsigned h=geom.h;
	if(items_part)
	{
		const Sides &margin=items_part->get_margin();
		h-=margin.top+margin.bottom;
	}

	n_visible=h/row_height;

	if(first+n_visible>items.size())
	{
		if(items.size()>n_visible)
			first=items.size()-n_visible;
		else
			first=0;
	}

	if(items.size()>n_visible)
	{
		slider.set_range(0, items.size()-n_visible);
		slider.set_value(items.size()-n_visible-first);
	}
	else
	{
		slider.set_range(0, 0);
		slider.set_value(0);
	}
}

void List::slider_value_changed(double value)
{
	if(items.size()>n_visible)
		first=items.size()-n_visible-static_cast<unsigned>(value);
}


List::Loader::Loader(List &l):
	Widget::Loader(l)
{
	add("item", &Loader::item);
}

void List::Loader::item(const string &v)
{
	dynamic_cast<List &>(wdg).append(v);
}

} // namespace GLtk
} // namespace Msp
