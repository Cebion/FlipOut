/* $Id: executable.h 85 2009-02-21 00:45:50Z tdb $

This file is part of builder
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef EXECUTABLE_H_
#define EXECUTABLE_H_

#include "binary.h"

class Executable: public Binary
{
public:
	Executable(Builder &, const Component &, const std::list<ObjectFile *> &);
	virtual const char *get_type() const { return "Executable"; }
};

#endif
