/* $Id: widget.cpp 55 2009-09-20 16:27:08Z tdb $

This file is part of libmspgltk
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/immediate.h>
#include <msp/gl/matrix.h>
#include <msp/gl/transform.h>
#include <msp/strings/formatter.h>
#include "panel.h"
#include "resources.h"
#include "widget.h"

using namespace std;

namespace Msp {
namespace GLtk {

Widget::Widget(const Resources &r):
	res(r),
	style(0),
	state(NORMAL),
	visible(true),
	parent(0)
{ }

Widget::~Widget()
{
	if(parent)
		parent->remove(*this);
}

void Widget::set_position(int x, int y)
{
	geom.x=x;
	geom.y=y;
	on_geometry_change();
}

void Widget::set_size(unsigned w, unsigned h)
{
	geom.w=w;
	geom.h=h;
	on_geometry_change();
}

void Widget::set_geometry(const Geometry &g)
{
	geom=g;
	on_geometry_change();
}

void Widget::set_style(const string &s)
{
	style_name=s;
	update_style();
}

void Widget::set_tooltip(const string &t)
{
	tooltip = t;
}

void Widget::set_visible(bool v)
{
	if(v==visible)
		return;

	visible=v;

	signal_visibility_changed.emit(visible);
}

void Widget::set_focus()
{
	if(!parent)
		throw InvalidState("No parent");
	if(!visible)
		throw InvalidState("Can't set focus on invisible widget");

	signal_request_focus.emit();
}

void Widget::render() const
{
	if(!style)
		throw InvalidState(format("Attempt to render a widget without a style (class=\"%s\")", get_class()));

	GL::push_matrix();
	GL::translate(geom.x, geom.y, 0);
	for(PartSeq::const_iterator i=style->get_parts().begin(); i!=style->get_parts().end(); ++i)
	{
		if(i->get_name().empty())
		{
			GL::PushMatrix push_;
			i->render(geom, state);
		}
			//render_graphic(*i);
		else
			render_special(*i);
	}
	GL::pop_matrix();
}

/*void Widget::render_graphic(const Part &part) const
{
	GL::push_matrix();
	part.render(geom, state);
	GL::pop_matrix();
}

void Widget::render_text(const Part &part, const string &text) const
{
	const GL::Font *const font=style->get_font();
	const float font_size=font->get_default_size();

	Geometry rgeom;
	rgeom.w=static_cast<unsigned>(font->get_string_width(text)*font_size);
	rgeom.h=static_cast<unsigned>((font->get_ascent()-font->get_descent())*font_size);
	rgeom.y=static_cast<int>(-font->get_descent()*font_size);
	part.get_alignment().apply(rgeom, geom, part.get_margin());

	GL::push_matrix();
	GL::translate(rgeom.x, rgeom.y, 0);
	GL::scale_uniform(font_size);

	const GL::Color &color=style->get_font_color();
	GL::Immediate imm((GL::COLOR4_UBYTE, GL::TEXCOORD2, GL::VERTEX2));
	imm.color(color.r, color.g, color.b);
	font->draw_string(text, imm);

	GL::pop_matrix();
}*/

void Widget::pointer_enter()
{
	state|=HOVER;
}

void Widget::pointer_leave()
{
	state&=~HOVER;
}

void Widget::focus_in()
{
	state|=FOCUS;
}

void Widget::focus_out()
{
	state&=~FOCUS;
}

void Widget::update_style()
{
	string sname=get_class();
	if(!style_name.empty())
	{
		sname+='-';
		sname+=style_name;
	}
	style=res.get<Style>(sname);
	on_style_change();
}

void Widget::set_parent(Container *p)
{
	if(parent && p)
		throw InvalidState("Widget is already in a Container");
	parent=p;

	on_reparent();
}

void Widget::set_parent(Widget &w, Container *p)
{
	w.set_parent(p);
}


Widget::Loader::Loader(Widget &w):
	wdg(w)
{
	add("position", &Loader::position);
	add("size",     &Loader::size);
	add("style",    &Loader::style);
	add("visible",  &Widget::visible);
}

void Widget::Loader::position(int x, int y)
{
	wdg.set_position(x, y);
}

void Widget::Loader::size(unsigned w, unsigned h)
{
	wdg.set_size(w, h);
}

void Widget::Loader::style(const string &s)
{
	wdg.set_style(s);
}

} // namespace GLtk
} // namespace Msp
