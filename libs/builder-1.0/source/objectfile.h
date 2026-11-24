/* $Id: objectfile.h 85 2009-02-21 00:45:50Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef OBJECTFILE_H_
#define OBJECTFILE_H_

#include "filetarget.h"

class Component;
class SourceFile;

/**
Object files are compiled from source files.
*/
class ObjectFile: public FileTarget
{
private:
	const Component &comp;
	SourceFile &source;
	TargetList new_deps;
	
public:
	ObjectFile(Builder &, const Component &, SourceFile &);
	virtual const char *get_type() const { return "ObjectFile"; }
	const Component &get_component() const { return comp; }
	SourceFile &get_source() const { return source; }

	/** Processes as many new dependences as possible.  Some may be created on
	the fly and can't be processed until their own dependencies are ready.  In
	such cases this function needs to be called again. */
	virtual void find_depends();

private:
	/** Recursively looks for header targets and adds them as dependencies. */
	void find_depends(Target *);

	/** Adds a target to the dependency list as well as the new dependencies
	list. */
	void add_depend(Target *);
	virtual Action *create_action();

	static Msp::FS::Path generate_target_path(const Component &, const std::string &);
};

#endif
