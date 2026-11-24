/* $Id: style.h 17 2007-12-11 13:30:49Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_STYLE_H_
#define MSP_GLTK_STYLE_H_

#include <msp/gl/color.h>
#include <msp/gl/font.h>
#include <msp/datafile/loader.h>
#include "part.h"

namespace Msp {
namespace GLtk {

class Resources;

/**
Styles define what wigets look like.  They are made up of Parts and some
generic properties.
*/
class Style
{
public:
	class Loader: public DataFile::Loader
	{
	private:
		Style &style;
		Resources &res;

	public:
		typedef Resources Collection;

		Loader(Style &, Resources &);
		Style &get_object() const { return style; }
		Resources &get_collection() const { return res; }
	private:
		void font(const std::string &);
		void font_color(float, float, float);
		void part();
		void special(const std::string &);
	};

private:
	const GL::Font *font;
	GL::Color font_color;
	PartSeq parts;

public:
	Style(Resources &);
	const GL::Font *get_font() const  { return font; }
	const GL::Color &get_font_color() const { return font_color; }
	const PartSeq &get_parts() const { return parts; }
};

} // namespace GLtk
} // namespace Msp

#endif
