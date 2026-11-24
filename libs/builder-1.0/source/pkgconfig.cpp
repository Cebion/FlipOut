/* $Id: pkgconfig.cpp 85 2009-02-21 00:45:50Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "package.h"
#include "pkgconfig.h"
#include "pkgconfigaction.h"

PkgConfig::PkgConfig(Builder &b, const SourcePackage &p):
	FileTarget(b, &p, p.get_source()/(p.get_name()+".pc"))
{
	buildable=true;
}

Action *PkgConfig::create_action()
{
	return new PkgConfigAction(builder, *this);
}
