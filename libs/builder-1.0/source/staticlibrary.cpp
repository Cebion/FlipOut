/* $Id: staticlibrary.cpp 85 2009-02-21 00:45:50Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "archive.h"
#include "component.h"
#include "objectfile.h"
#include "sourcepackage.h"
#include "staticlibrary.h"

using namespace std;

StaticLibrary::StaticLibrary(Builder &b, const Component &c, const list<ObjectFile *> &objs):
	FileTarget(b, &c.get_package(), generate_target_path(c)),
	Library(b, package, path, c.get_name()),
	comp(c)
{
	buildable=true;
	for(list<ObjectFile *>::const_iterator i=objs.begin(); i!=objs.end(); ++i)
		add_depend(*i);
}

Action *StaticLibrary::create_action()
{
	return new Archive(builder, *this);
}

Msp::FS::Path StaticLibrary::generate_target_path(const Component &c)
{
	return c.get_package().get_out_dir()/("lib"+c.get_name()+".a");
}
