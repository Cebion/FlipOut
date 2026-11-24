/* $Id: entry.cpp 55 2009-09-20 16:27:08Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/matrix.h>
#include <msp/gl/texture.h>
#include <msp/gl/transform.h>
#include <msp/input/keys.h>
#include "entry.h"
#include "graphic.h"
#include "part.h"
#include "style.h"

using namespace std;

namespace Msp {
namespace GLtk {

Entry::Entry(const Resources &r, const string &t):
	Widget(r),
	text(style),
	edit_pos(0)
{
	update_style();
	set_text(t);
}

void Entry::set_text(const string &t)
{
	text=t;
	edit_pos=text.size();
}

void Entry::key_press(unsigned key, unsigned, wchar_t ch)
{
	if(key==Input::KEY_LEFT)
	{
		if(edit_pos>0)
			--edit_pos;
	}
	else if(key==Input::KEY_RIGHT)
	{
		if(edit_pos<text.size())
			++edit_pos;
	}
	else if(key==Input::KEY_BACKSPACE)
	{
		if(edit_pos>0)
			text.erase(--edit_pos, 1);
	}
	else if(key==Input::KEY_ENTER)
		signal_enter.emit();
	else if(ch>=' ')
	{
		text.insert(edit_pos, Codecs::encode<Codecs::Utf8>(Codecs::ustring(1, ch)));
		++edit_pos;
	}
}

void Entry::render_special(const Part &part) const
{
	if(part.get_name()=="text")
		text.render(part, geom);
	else if(part.get_name()=="cursor")
	{
		if(!part.get_graphic(state))
			return;

		const GL::Font *const font=style->get_font();
		const float font_size=font->get_default_size();

		Geometry rgeom=part.get_geometry();
		rgeom.x=static_cast<unsigned>(font->get_string_width(text.get().substr(0, edit_pos))*font_size);
		rgeom.w=static_cast<unsigned>(font->get_string_width(text.get())*font_size);
		part.get_alignment().apply(rgeom, geom, part.get_margin());

		GL::push_matrix();
		GL::translate(rgeom.x, rgeom.y, 0);
		part.get_graphic(state)->render(part.get_geometry().w, rgeom.h);
		GL::pop_matrix();
	}
}


Entry::Loader::Loader(Entry &ent):
	Widget::Loader(ent)
{ }

} // namespace GLtk
} // namespace Msp
