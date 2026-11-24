/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/datafile/writer.h>
#include <msp/fs/dir.h>
#include <msp/fs/stat.h>
#include <msp/fs/utils.h>
#include "settings.h"

using namespace std;
using namespace Msp;

Settings::Settings():
	filename(FS::get_user_data_dir("flipout")/"conf"),
	width(800),
	height(600),
	fullscreen(false),
	vertex_buffers(true),
	simple_models(false),
	use_shaders(true),
	glow(true),
	reflection(true),
	shadows(true),
	shadow_quality(9),
	difficulty(1),
	explosions(true),
	physics_quality(2),
	optimize_physics(true),
	sensitivity(7),
	speed(6),
	music_enabled(true),
	music_volume(100),
	sfx_enabled(true),
	sfx_volume(100)
{
	by_name["fullscreen"]=&fullscreen;
	by_name["vertex_buffers"]=&vertex_buffers;
	by_name["simple_models"]=&simple_models;
	by_name["use_shaders"]=&use_shaders;
	by_name["glow"]=&glow;
	by_name["reflection"]=&reflection;
	by_name["shadows"]=&shadows;
	by_name["shadow_quality"]=&shadow_quality;

	by_name["difficulty"]=&difficulty;
	by_name["explosions"]=&explosions;
	by_name["physics_quality"]=&physics_quality;
	by_name["optimize_physics"]=&optimize_physics;

	by_name["sensitivity"]=&sensitivity;
	by_name["speed"]=&speed;

	by_name["music_enabled"]=&music_enabled;
	by_name["music_volume"]=&music_volume;
	by_name["sfx_enabled"]=&sfx_enabled;
	by_name["sfx_volume"]=&sfx_volume;
}

const Variant &Settings::get(const string &name)
{
	map<string, Variant>::iterator i=by_name.find(name);
	if(i==by_name.end())
		throw KeyError("Unknown setting", name);
	return i->second;
}

void Settings::load()
{
	if(FS::is_reg(filename))
		DataFile::load(*this, filename.str());
}

void Settings::save() const
{
	FS::Path dir=FS::dirname(filename);
	if(!exists(dir))
		FS::mkdir(dir, 0755);

	IO::BufferedFile out(filename.str(), IO::M_WRITE);
	DataFile::Writer writer(out);

	writer.write((DataFile::Statement("resolution"), width, height));
	writer.write((DataFile::Statement("fullscreen"), fullscreen));
	writer.write((DataFile::Statement("vertex_buffers"), vertex_buffers));
	writer.write((DataFile::Statement("simple_models"), simple_models));
	writer.write((DataFile::Statement("use_shaders"), use_shaders));
	writer.write((DataFile::Statement("glow"), glow));
	writer.write((DataFile::Statement("reflection"), reflection));
	writer.write((DataFile::Statement("shadows"), shadows));
	writer.write((DataFile::Statement("shadow_quality"), shadow_quality));
	writer.write((DataFile::Statement("difficulty"), difficulty));
	writer.write((DataFile::Statement("explosions"), explosions));
	writer.write((DataFile::Statement("physics_quality"), physics_quality));
	writer.write((DataFile::Statement("optimize_physics"), optimize_physics));
	for(map<string, unsigned>::const_iterator i=controls.begin(); i!=controls.end(); ++i)
		writer.write((DataFile::Statement("control"), i->first, i->second));
	writer.write((DataFile::Statement("sensitivity"), sensitivity));
	writer.write((DataFile::Statement("speed"), speed));
	writer.write((DataFile::Statement("music_enabled"), music_enabled));
	writer.write((DataFile::Statement("music_volume"), music_volume));
	writer.write((DataFile::Statement("sfx_enabled"), sfx_enabled));
	writer.write((DataFile::Statement("sfx_volume"), sfx_volume));
}


Settings::Loader::Loader(Settings &s):
	sett(s)
{
	add("resolution",       &Settings::width, &Settings::height);
	add("fullscreen",       &Settings::fullscreen);
	add("vertex_buffers",   &Settings::vertex_buffers);
	add("simple_models",    &Settings::simple_models);
	add("use_shaders",      &Settings::use_shaders);
	add("glow",             &Settings::glow);
	add("reflection",       &Settings::reflection);
	add("shadows",          &Settings::shadows);
	add("shadow_quality",   &Settings::shadow_quality);
	add("difficulty",       &Settings::difficulty);
	add("explosions",       &Settings::explosions);
	add("physics_quality",  &Settings::physics_quality);
	add("optimize_physics", &Settings::optimize_physics);
	add("control",          &Loader::control);
	add("sensitivity",      &Settings::sensitivity);
	add("speed",            &Settings::speed);
	add("music_enabled",    &Settings::music_enabled);
	add("music_volume",     &Settings::music_volume);
	add("sfx_enabled",      &Settings::sfx_enabled);
	add("sfx_volume",       &Settings::sfx_volume);
}

void Settings::Loader::control(const string &name, unsigned src)
{
	sett.controls[name]=src;
}
