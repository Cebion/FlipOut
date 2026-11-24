/* $Id$

This file is part of flipout
Copyright © 2007-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include <msp/gl/programdata.h>
#include <msp/gl/texture.h>

class Resources;

class Background
{
public:
	class Loader: public Msp::DataFile::Loader
	{
	public:
		typedef Resources Collection;

	private:
		Background &bg;
		Collection &coll;

	public:
		Loader(Background &, Collection &);
		Background &get_object() const { return bg; }
		Collection &get_collection() const { return coll; }
	private:
		virtual void finish();
	};

private:
	Msp::GL::Program *shader;
	Msp::GL::ProgramData *shdata;
	const Msp::GL::Texture *tex;
	float reflect;

public:
	Background();
	~Background();

	Msp::GL::Program *get_shader() const { return shader; }
	const Msp::GL::Texture *get_texture() const { return tex; }
	float get_reflect() const { return reflect; }
	void apply(bool) const;
};

#endif
