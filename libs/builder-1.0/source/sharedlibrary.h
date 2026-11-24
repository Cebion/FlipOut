/* $Id: sharedlibrary.h 85 2009-02-21 00:45:50Z tdb $

This file is part of builder
Copyright © 2006-2007, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef SHAREDLIB_H_
#define SHAREDLIB_H_

#include "binary.h"
#include "library.h"

/**
Represents a shared library.  Mainly exists to give extra information to the
user.
*/
class SharedLibrary: public Binary, public Library
{
public:
	SharedLibrary(Builder &, const Component &, const std::list<ObjectFile *> &);
	virtual const char *get_type() const { return "SharedLibrary"; }
};

#endif
