/* $Id: graphic.h 14 2007-11-27 08:41:36Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_GRAPHIC_H_
#define MSP_GLTK_GRAPHIC_H_

#include <msp/gl/texture2d.h>
#include <msp/datafile/loader.h>
#include "geometry.h"

namespace Msp {
namespace GLtk {

class Resources;

/**
Stores a single graphical element.  Graphics are used as parts of widgets.
*/
class Graphic
{
public:
	class Loader: public DataFile::Loader
	{
	private:
		Graphic &graph;
		Resources &res;

	public:
		typedef Resources Collection;

		Loader(Graphic &, Resources &);
		Graphic &get_object() const { return graph; }
	private:
		void texture(const std::string &);
		void slice(unsigned, unsigned, unsigned, unsigned);
		void border();
		void shadow();
	};

private:
	Sides border;
	Sides shadow;
	const GL::Texture2D *texture;
	Geometry slice;
	bool repeat;

public:
	Graphic();
	const Sides &get_border() const { return border; }
	const Sides &get_shadow() const { return shadow; }
	const GL::Texture2D *get_texture() const { return texture; }
	unsigned get_width() const  { return slice.w; }
	unsigned get_height() const { return slice.h; }
	void render(unsigned, unsigned) const;
private:
	void create_coords(float, float, float, float, float, std::vector<float> &) const;
	void create_texcoords(float, float, float, float, float, std::vector<float> &) const;
};

} // namespace GLtk
} // namespace Msp

#endif
