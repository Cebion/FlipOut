/* $Id: datafile.h 98 2009-09-20 17:34:22Z tdb $

This file is part of builder
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef DATAFILE_H_
#define DATAFILE_H_

#include "filetarget.h"

class Component;
class File;

class DataFile: public FileTarget
{
private:
	const Component &component;
	File &source;

public:
	DataFile(Builder &, const Component &, File &);

	virtual const char *get_type() const { return "DataFile"; }
	const Component &get_component() const { return component; }
	File &get_source() const { return source; }

private:
	virtual Action *create_action();
	static Msp::FS::Path generate_target_path(const Component &);
};

#endif
