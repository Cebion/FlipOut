/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef RESOURCES_H_
#define RESOURCES_H_

#include <set>
#include <msp/al/buffer.h>
#include <msp/datafile/collection.h>
#include <msp/fs/path.h>
#include <msp/gl/mesh.h>
#include <msp/gl/program.h>
#include <msp/gl/texture.h>
#include <msp/gltk/logic.h>
#include <msp/gltk/resources.h>
#include <msp/gltk/root.h>

class BallType;
class BonusType;
class BrickType;
class FlipOut;
class Level;
class PaddleType;

class Resources: public Msp::DataFile::Collection
{
private:
	FlipOut &flipout;
	Msp::FS::Path path;
	Msp::GLtk::Resources ui_res;

public:
	class Loader: public Collection::Loader
	{
	private:
		Resources &res;

	public:
		Loader(Resources &);
	private:
		void shader(const std::string &);
		void ui_resources();
		void userinterface(const std::string &);
	};

	Resources(FlipOut &);
	~Resources();

	void load(const Msp::FS::Path &);
	const Msp::GLtk::Resources &get_ui_resources() const { return ui_res; }
private:
	Level *load_level(const std::string &);
	Msp::GL::Mesh *load_mesh(const std::string &);
	Msp::GL::Texture *load_texture(const std::string &);
	Msp::GL::Program *load_shader(const std::string &);
	Msp::AL::Buffer *load_sound(const std::string &);
};

#endif
