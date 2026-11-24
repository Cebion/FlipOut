/* $Id: systemlibrary.h 85 2009-02-21 00:45:50Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef SYSTEMLIBRARY_H_
#define SYSTEMLIBRARY_H_

#include "library.h"

/**
A library that doesn't belong to any known package.
*/
class SystemLibrary: public Library
{
public:
	SystemLibrary(Builder &, const Msp::FS::Path &);
	virtual const char *get_type() const { return "SystemLibrary"; }
private:
	virtual Action *create_action() { return 0; }

	static std::string extract_libname(const Msp::FS::Path &);
};

#endif
