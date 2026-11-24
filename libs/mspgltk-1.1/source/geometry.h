/* $Id: geometry.h 20 2007-12-25 20:41:08Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_GEOMETRY_H_
#define MSP_GLTK_GEOMETRY_H_

#include <msp/datafile/loader.h>

namespace Msp {
namespace GLtk {

/**
Specifies the position and size of a widget or graphic.
*/
struct Geometry
{
	int x, y;
	unsigned w, h;

	Geometry(): x(0), y(0), w(1), h(1) { }
	Geometry(int x_, int y_, unsigned w_, unsigned h_): x(x_), y(y_), w(w_), h(h_) { }
	bool is_inside(int, int) const;
	bool is_inside_relative(int, int) const;
};


/**
Specifies margins on the sides of an element.
*/
struct Sides
{
	class Loader: public DataFile::Loader
	{
	public:
		Loader(Sides &);
		Sides &get_object() { return sides; }
	private:
		Sides &sides;
	};

	unsigned top;
	unsigned right;
	unsigned bottom;
	unsigned left;

	Sides();
};


/**
Performs alignment of nested geometries, such as widgets and their parts.
*/
struct Alignment
{
	float x, y;
	float w, h;

	Alignment(): x(0), y(0), w(1), h(1) { }
	void apply(Geometry &, const Geometry &) const;
	void apply(Geometry &, const Geometry &, const Sides &) const;
};

} // namespace GLtk
} // namespace Msp

#endif
