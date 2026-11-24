/* $Id: style.cpp 17 2007-12-11 13:30:49Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "resources.h"
#include "style.h"

using namespace std;

namespace Msp {
namespace GLtk {

Style::Style(Resources &r):
	font(&r.get_default_font())
{ }


Style::Loader::Loader(Style &s, Resources &r):
	style(s),
	res(r)
{
	add("font",       &Style::font);
	add("font_color", &Loader::font_color);
	add("part",       &Loader::part);
	add("special",    &Loader::special);
}

void Style::Loader::font_color(float r, float g, float b)
{
	style.font_color=GL::Color(r, g, b);
}

void Style::Loader::part()
{
	Part p((string()));
	load_sub(p, res);
	style.parts.push_back(p);
}

void Style::Loader::special(const string &n)
{
	Part p(n);
	load_sub(p, res);
	style.parts.push_back(p);
}

} // namespace GLtk
} // namespace Msp
