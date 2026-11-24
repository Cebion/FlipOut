/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cstdlib>
#include <msp/core/refptr.h>
#include <msp/gl/except.h>
#include <msp/gl/material.h>
#include <msp/gl/shader.h>
#include <msp/gl/technique.h>
#include <msp/gl/texture2d.h>
#include <msp/gl/texture3d.h>
#include <msp/gltk/userinterface.h>
#include <msp/fs/dir.h>
#include "award.h"
#include "background.h"
#include "balltype.h"
#include "bonustype.h"
#include "bricktype.h"
#include "colorscheme.h"
#include "flipout.h"
#include "level.h"
#include "paddletype.h"
#include "resources.h"
#include "series.h"

using namespace std;
using namespace Msp;

Resources::Resources(FlipOut &fo):
	flipout(fo)
{
	add_keyword<Award>("award");
	add_keyword<Background>("background");
	add_keyword<BallType>("ball");
	add_keyword<BonusType>("bonus");
	add_keyword<BrickType>("brick");
	add_keyword<ColorScheme>("color_scheme");
	add_keyword<Level>("level");
	add_keyword<GL::Material>("material");
	add_keyword<GL::Mesh>("mesh");
	add_keyword<PaddleType>("paddle");
	add_keyword<Series>("series");
	add_keyword<GL::Program>("shader");
	add_keyword<AL::Buffer>("sound");
	add_keyword<GL::Technique>("technique");
	add_keyword<GL::Texture2D, GL::Texture>("texture");

	add_creator(&Resources::load_level);
	add_creator(&Resources::load_mesh);
	add_creator(&Resources::load_texture);
	add_creator(&Resources::load_shader);
	add_creator(&Resources::load_sound);
}

Resources::~Resources()
{ }

void Resources::load(const FS::Path &p)
{
	path=p;
	ui_res.set_path(path);

	list<string> files=FS::list_filtered(path, "\\.data$");
	files.sort();
	for(list<string>::const_iterator i=files.begin(); i!=files.end(); ++i)
		DataFile::load(*this, (path / *i).str());
}

Level *Resources::load_level(const string &name)
{
	RefPtr<Level> level=new Level;
	DataFile::load(*level, (path/name).str(), *this);
	return level.release();
}

GL::Mesh *Resources::load_mesh(const string &name)
{
	RefPtr<GL::Mesh> mesh=new GL::Mesh;
	DataFile::load(*mesh, (path/name).str());
	return mesh.release();
}

GL::Texture *Resources::load_texture(const string &name)
{
	if(!name.compare(0, 7, "noise3d"))
	{
		RefPtr<GL::Texture3D> tex3d=new GL::Texture3D;
		char data[64*64*64];
		for(unsigned i=0; i<64*64*64; ++i)
			data[i]=rand();
		tex3d->storage(GL::LUMINANCE, 64, 64, 64, 0);
		tex3d->image(0, GL::LUMINANCE, GL::UNSIGNED_BYTE, data);
		tex3d->set_min_filter(GL::LINEAR);
		return tex3d.release();
	}
	else if(!name.compare(0, 5, "noise"))
	{
		RefPtr<GL::Texture2D> tex2d=new GL::Texture2D;
		char data[64*64];
		for(unsigned i=0; i<64*64; ++i)
			data[i]=rand();
		tex2d->storage(GL::LUMINANCE, 64, 64, 0);
		tex2d->image(0, GL::LUMINANCE, GL::UNSIGNED_BYTE, data);
		tex2d->set_min_filter(GL::LINEAR);
		return tex2d.release();
	}
	else
	{
		RefPtr<GL::Texture2D> tex2d=new GL::Texture2D;
		tex2d->load_image((path/name).str());
		tex2d->set_min_filter(GL::LINEAR);
		return tex2d.release();
	}
}

GL::Program *Resources::load_shader(const string &name)
{
	try
	{
		RefPtr<GL::Program> shd=new GL::Program;
		DataFile::load(*shd, (path/name).str());
		return shd.release();
	}
	catch(const GL::UnsupportedExtension &)
	{
		return 0;
	}
}

AL::Buffer *Resources::load_sound(const string &name)
{
	RefPtr<AL::Buffer> snd=new AL::Buffer;
	snd->load_data((path/name).str());
	return snd.release();
}


Resources::Loader::Loader(Resources &r):
	Collection::Loader(r),
	res(r)
{
	add("shader",       &Loader::shader);
	add("ui_resources", &Loader::ui_resources);
	add("userinterface", &Loader::userinterface);
}

void Resources::Loader::shader(const string &n)
{
	try
	{
		RefPtr<GL::Program> shd=new GL::Program;
		load_sub(*shd);
		res.add(n, shd.get());
		shd.release();
	}
	catch(const GL::UnsupportedExtension &)
	{
		res.add<GL::Program>(n, 0);
	}
}

void Resources::Loader::ui_resources()
{
	load_sub(res.ui_res);
}

void Resources::Loader::userinterface(const string &n)
{
	RefPtr<GLtk::UserInterface> ui=new GLtk::UserInterface(res.ui_res, res.flipout.get_window());
	load_sub(*ui);
	res.add(n, ui.get());
	ui.release();
}
