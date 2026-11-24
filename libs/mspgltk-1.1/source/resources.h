/* $Id: resources.h 34 2008-07-20 12:40:21Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_RESOURCES_H_
#define MSP_GLTK_RESOURCES_H_

#include <msp/gl/font.h>
#include <msp/gl/texture.h>
#include <msp/datafile/collection.h>
#include <msp/fs/path.h>
#include "graphic.h"
#include "style.h"

namespace Msp {
namespace GLtk {

/**
Stores resources such as styles and graphics.  All widgets require a Resources
instance in constructor.
*/
class Resources: public DataFile::Collection
{
private:
	FS::Path path;
	GL::Font *default_font;

public:
	class Loader: public Collection::Loader
	{
	private:
		Resources &res;

	public:
		Loader(Resources &);
	private:
		void default_font(const std::string &);
		void font(const std::string &);
		void style(const std::string &);
	};

	Resources();

	void set_path(const FS::Path &);
	const GL::Font &get_default_font() const;
private:
	GL::Font *create_font(const std::string &);
	GL::Texture2D *create_texture(const std::string &);
};

} // namespace GLtk
} // namespace Msp

#endif
