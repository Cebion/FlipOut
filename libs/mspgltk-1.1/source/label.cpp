/* $Id: label.cpp 55 2009-09-20 16:27:08Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "label.h"
#include "part.h"
#include "style.h"

using namespace std;

namespace Msp {
namespace GLtk {

Label::Label(const Resources &r, const string &t):
	Widget(r),
	text(style)
{
	update_style();
	set_text(t);
}

void Label::autosize()
{
	const list<Part> &parts=style->get_parts();
	const Part *text_part=0;
	for(list<Part>::const_iterator i=parts.begin(); (!text_part && i!=parts.end()); ++i)
		if(i->get_name()=="text")
			text_part=&*i;

	geom.h=text.get_height();
	geom.w=text.get_width();
	if(text_part)
	{
		const Sides &margin=text_part->get_margin();
		geom.w+=margin.left+margin.right;
		geom.h+=margin.top+margin.bottom;
	}
}

void Label::set_text(const string &t)
{
	text=t;
}

void Label::render_special(const Part &part) const
{
	if(part.get_name()=="text")
		text.render(part, geom);
}


Label::Loader::Loader(Label &l):
	Widget::Loader(l)
{
	add("text", &Loader::text);
}

void Label::Loader::text(const string &t)
{
	static_cast<Label &>(wdg).text=t;
}

} // namespace GLtk
} // namespace Msp
