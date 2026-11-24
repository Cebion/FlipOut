/* $Id: binary.h 85 2009-02-21 00:45:50Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef BINARY_H_
#define BINARY_H_

#include "filetarget.h"

class Component;
class ObjectFile;

/**
Produces a binary file, which may be either a standalone executable or a shared
library.
*/
class Binary: public virtual FileTarget
{
protected:
	const Component &comp;

	Binary(Builder &, const Component &, const std::list<ObjectFile *> &);
public:
	const Component &get_component() const { return comp; }
	virtual void find_depends();
protected:
	virtual Action *create_action();

	/** Returns the path for the binary.  We can't do this in the constructor
	since we need to pass the value to the Target c'tor. */
	static Msp::FS::Path generate_target_path(const Component &);
};

#endif
