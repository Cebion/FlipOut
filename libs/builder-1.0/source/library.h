/* $Id: library.h 85 2009-02-21 00:45:50Z tdb $

This file is part of builder
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef LIBRARY_H_
#define LIBRARY_H_

#include "filetarget.h"

class Library: public virtual FileTarget
{
protected:
	std::string libname;

	Library(Builder &, const Package *, const Msp::FS::Path &, const std::string &);
public:
	const std::string &get_libname() const { return libname; }
};

#endif
