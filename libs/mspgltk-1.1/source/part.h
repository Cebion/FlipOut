/* $Id: part.h 20 2007-12-25 20:41:08Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_PART_H_
#define MSP_GLTK_PART_H_

#include <map>
#include <string>
#include <msp/datafile/loader.h>
//#include "alignment.h"
#include "geometry.h"
#include "state.h"

namespace Msp {
namespace GLtk {

class Graphic;
class Resources;

/**
Defines a single graphical element of a widget style.
*/
class Part
{
public:
	class Loader: public DataFile::Loader
	{
	private:
		Part &part;
		Resources &res;

	public:
		Loader(Part &, Resources &);
		~Loader();
	private:
		void graphic(State, const std::string &);
		void align(float, float);
		void fill(float, float);
		void margin();
		void size(unsigned, unsigned);
	};

private:
	std::string name;
	const Graphic *graphic[N_STATES_];
	Geometry geom;
	Sides margin;
	Alignment align;

public:
	Part(const std::string &);
	const std::string &get_name() const { return name; }
	const Graphic *get_graphic(State) const;
	const Geometry &get_geometry() const { return geom; }
	const Sides &get_margin() const { return margin; }
	const Alignment &get_alignment() const { return align; }
	void render(const Geometry &, State) const;
};
typedef std::list<Part> PartSeq;

} // namespace GLtk
} // namespace Msp

#endif
