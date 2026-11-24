/* $Id: sharedlibrary.cpp 85 2009-02-21 00:45:50Z tdb $

This file is part of builder
Copyright © 2006-2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "component.h"
#include "sharedlibrary.h"
#include "sourcepackage.h"

using namespace std;

SharedLibrary::SharedLibrary(Builder &b, const Component &c, const list<ObjectFile *> &objs):
	FileTarget(b, &c.get_package(), generate_target_path(c)),
	Binary(b, c, objs),
	Library(b, &c.get_package(), path, c.get_name())
{ }
