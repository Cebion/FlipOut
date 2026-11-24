/* $Id: sourcefile.h 96 2009-09-11 22:48:06Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef SOURCEFILE_H_
#define SOURCEFILE_H_

#include "filetarget.h"

class Component;

/**
Represents a C or C++ source file.
*/
class SourceFile: public FileTarget
{
private:
	const Component *comp;
	StringList includes;

protected:
	SourceFile(Builder &, const Msp::FS::Path &);
public:
	SourceFile(Builder &, const Component &, const Msp::FS::Path &);
	virtual const char *get_type() const { return "SourceFile"; }
	const StringList &get_includes() const { return includes; }
	const Component *get_component() const { return comp; }
	virtual void find_depends();
private:
	virtual Action *create_action() { return 0; }
};

#endif
