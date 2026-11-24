/* $Id: resources.cpp 37 2008-07-23 00:07:31Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/core/except.h>
#include "resources.h"

using namespace std;

namespace Msp {
namespace GLtk {

Resources::Resources():
	path("."),
	default_font(0)
{
	add_keyword<Graphic>("graphic");
	add_keyword<GL::Texture2D>("texture");

	add_creator(&Resources::create_font);
	add_creator(&Resources::create_texture);
}

void Resources::set_path(const FS::Path &p)
{
	path=p;
}

const GL::Font &Resources::get_default_font() const
{
	if(!default_font)
		throw InvalidState("No default font");

	return *default_font;
}

GL::Font *Resources::create_font(const string &name)
{
	RefPtr<GL::Font> fnt=new GL::Font;
	DataFile::load(*fnt, (path/name).str(), *this);
	if(!default_font)
		default_font=fnt.get();
	return fnt.release();
}

GL::Texture2D *Resources::create_texture(const string &name)
{
	RefPtr<GL::Texture2D> tex=new GL::Texture2D;
	tex->load_image((path/name).str());
	tex->set_min_filter(GL::LINEAR);
	return tex.release();
}


Resources::Loader::Loader(Resources &r):
	Collection::Loader(r),
	res(r)
{
	add("default_font", &Loader::default_font);
	add("font", &Loader::font);
	add("style", &Loader::style);
}

void Resources::Loader::default_font(const string &name)
{
	res.default_font=res.get<GL::Font>(name);
}

void Resources::Loader::font(const string &name)
{
	RefPtr<GL::Font> fnt=new GL::Font;
	load_sub(*fnt, res);
	res.add(name, fnt.get());
	if(!res.default_font)
		res.default_font=fnt.get();
	fnt.release();
}

void Resources::Loader::style(const string &name)
{
	RefPtr<Style> stl=new Style(res);
	load_sub(*stl, res);
	res.add(name, stl.get());
	stl.release();
}

} // namespace GLtk
} // namespace Msp
