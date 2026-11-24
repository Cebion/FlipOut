/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/core/except.h>
#include "colorscheme.h"

using namespace std;
using namespace Msp;

void ColorScheme::add(const string &n, const BrickColor &c)
{
	if(colors.count(n))
		throw KeyError("Duplicate color name", n);

	colors[n]=c;
}

void ColorScheme::add(const ColorScheme &scheme)
{
	for(map<string, BrickColor>::const_iterator i=scheme.colors.begin(); i!=scheme.colors.end(); ++i)
		add(i->first, i->second);
}

const BrickColor &ColorScheme::get(const string &n) const
{
	if(n.empty())
		return colors.begin()->second;

	map<string, BrickColor>::const_iterator i=colors.find(n);
	if(i==colors.end())
		throw KeyError("Unknown color", n);
	return i->second;
}


ColorScheme::Loader::Loader(ColorScheme &s):
	scheme(s)
{
	add("color", &Loader::color);
}

void ColorScheme::Loader::color(const string &n)
{
	BrickColor bc;
	load_sub(bc);
	scheme.add(n, bc);
}
