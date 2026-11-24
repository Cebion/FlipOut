/* $Id: button.cpp 55 2009-09-20 16:27:08Z tdb $

This file is part of libmspgltk
Copyright © 2007, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/immediate.h>
#include "button.h"
#include "part.h"

namespace Msp {
namespace GLtk {

Button::Button(const Resources &r, const std::string &t):
	Widget(r),
	text(style),
	icon(0),
	pressed(false)
{
	update_style();
	set_text(t);
}

void Button::set_text(const std::string &t)
{
	text=t;
}

void Button::set_icon(const GL::Texture2D *i)
{
	icon=i;
}

void Button::button_press(int, int, unsigned btn)
{
	if(btn==1)
	{
		pressed=true;
		state|=ACTIVE;
	}
}

void Button::button_release(int x, int y, unsigned btn)
{
	if(pressed && btn==1)
	{
		state&=~ACTIVE;
		pressed=false;

		if(geom.is_inside_relative(x, y))
			signal_clicked.emit();
	}
}

void Button::pointer_motion(int x, int y)
{
	if(pressed)
	{
		if(!geom.is_inside_relative(x, y))
			state&=~ACTIVE;
		else
			state|=ACTIVE;
	}
}

void Button::render_special(const Part &part) const
{
	if(part.get_name()=="text")
		//render_text(part, text);
		text.render(part, geom);
	if(part.get_name()=="icon" && icon)
	{
		Geometry rgeom;
		rgeom.w=icon->get_width();
		rgeom.h=icon->get_height();
		part.get_alignment().apply(rgeom, geom, part.get_margin());

		icon->bind();
		GL::Immediate imm((GL::COLOR4_UBYTE, GL::TEXCOORD2, GL::VERTEX2));
		imm.color(1.0f, 1.0f, 1.0f);
		imm.begin(GL::QUADS);
		imm.texcoord(0, 0);
		imm.vertex(rgeom.x, rgeom.y);
		imm.texcoord(1, 0);
		imm.vertex(rgeom.x+rgeom.w, rgeom.y);
		imm.texcoord(1, 1);
		imm.vertex(rgeom.x+rgeom.w, rgeom.y+rgeom.h);
		imm.texcoord(0, 1);
		imm.vertex(rgeom.x, rgeom.y+rgeom.h);
		imm.end();
		GL::Texture::unbind();
	}
}


Button::Loader::Loader(Button &btn):
	Widget::Loader(btn)
{
	add("text", &Loader::text);
}

void Button::Loader::text(const std::string &t)
{
	static_cast<Button &>(wdg).text=t;
}

} // namespace GLtk
} // namespace Msp
