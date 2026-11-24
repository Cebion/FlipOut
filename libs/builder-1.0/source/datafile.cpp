/* $Id: datafile.cpp 98 2009-09-20 17:34:22Z tdb $

This file is part of builder
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "component.h"
#include "datacompile.h"
#include "datafile.h"
#include "file.h"
#include "sourcepackage.h"

DataFile::DataFile(Builder &b, const Component &c, File &s):
	FileTarget(b, &c.get_package(), generate_target_path(c)),
	component(c),
	source(s)
{
	buildable=true;
	add_depend(&source);
}

Action *DataFile::create_action()
{
	return new DataCompile(builder, *this);
}

Msp::FS::Path DataFile::generate_target_path(const Component &comp)
{
	return comp.get_package().get_out_dir()/(comp.get_name()+".dat");
}
