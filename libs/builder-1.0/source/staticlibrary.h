/* $Id: staticlibrary.h 85 2009-02-21 00:45:50Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef STATICLIB_H_
#define STATICLIB_H_

#include "library.h"

class Component;
class ObjectFile;

/**
A static library target.
*/
class StaticLibrary: public Library
{
private:
	const Component &comp;

public:
	StaticLibrary(Builder &, const Component &, const std::list<ObjectFile *> &);
	virtual const char *get_type() const { return "StaticLibrary"; }
	const Component &get_component() const { return comp; }
private:
	virtual Action *create_action();

	static Msp::FS::Path generate_target_path(const Component &);
};

#endif
