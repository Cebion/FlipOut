/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <msp/core/variant.h>
#include <msp/datafile/loader.h>
#include <msp/fs/path.h>

class Settings
{
public:
	class Loader: public Msp::DataFile::Loader
	{
	private:
		Settings &sett;
	
	public:
		Loader(Settings &);
		Settings &get_object() const { return sett; }
	private:
		void control(const std::string &, unsigned);
	};

	Msp::FS::Path filename;

	unsigned width;
	unsigned height;
	bool fullscreen;
	bool vertex_buffers;
	bool simple_models;
	bool use_shaders;
	bool glow;
	bool reflection;
	bool shadows;
	unsigned shadow_quality;
	unsigned difficulty;
	bool explosions;
	unsigned physics_quality;
	bool optimize_physics;
	std::map<std::string, unsigned> controls;
	unsigned sensitivity;
	unsigned speed;
	bool music_enabled;
	unsigned music_volume;
	bool sfx_enabled;
	unsigned sfx_volume;

	std::map<std::string, Msp::Variant> by_name;

	Settings();
	const Msp::Variant &get(const std::string &);
	void load();
	void save() const;
};

#endif
