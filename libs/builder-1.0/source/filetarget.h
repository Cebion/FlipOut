/* $Id: filetarget.h 102 2009-09-21 16:23:33Z tdb $

This file is part of builder
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef FILETARGET_H_
#define FILETARGET_H_

#include <msp/fs/path.h>
#include "target.h"

/**
An intermediate base class for targets that represent files.  Almost all target
classes are derived from this.
*/
class FileTarget: public Target
{
protected:
	Msp::FS::Path path;
	unsigned size;

	FileTarget(Builder &, const Package *, const Msp::FS::Path &);
public:
	const Msp::FS::Path &get_path() const { return path; }
	unsigned get_size() const { return size; }
};

#endif
