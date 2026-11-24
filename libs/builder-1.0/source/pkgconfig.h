/* $Id: pkgconfig.h 85 2009-02-21 00:45:50Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef PKGCONFIG_H_
#define PKGCONFIG_H_

#include "sourcepackage.h"
#include "filetarget.h"

/**
Creates a .pc file to enable other packages fetch build options with pkg-config.
*/
class PkgConfig: public FileTarget
{
public:
	PkgConfig(Builder &, const SourcePackage &);
	virtual const char *get_type() const { return "PkgConfig"; }
private:
	virtual Action *create_action();
};

#endif
