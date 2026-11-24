/* $Id: executable.cpp 85 2009-02-21 00:45:50Z tdb $

This file is part of builder
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "component.h"
#include "executable.h"
#include "sourcepackage.h"

Executable::Executable(Builder &b, const Component &c, const std::list<ObjectFile *> &objs):
	FileTarget(b, &c.get_package(), generate_target_path(c)),
	Binary(b, c, objs)
{ }
