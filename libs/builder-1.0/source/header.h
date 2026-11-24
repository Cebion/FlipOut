/* $Id: header.h 96 2009-09-11 22:48:06Z tdb $

This file is part of builder
Copyright © 2006-2007, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef HEADER_H_
#define HEADER_H_

#include "sourcefile.h"

/**
Represents a header file.  Mainly exists to give extra information to the user.
*/
class Header: public SourceFile
{
protected:
	Header(Builder &b, const std::string &f): SourceFile(b, f) { }
public:
	Header(Builder &b, const Component &c, const std::string &f): SourceFile(b, c, f) { }
	virtual const char *get_type() const { return "Header"; }
};

/**
A header file that doesn't belong to any known package.
*/
class SystemHeader: public Header
{
public:
	SystemHeader(Builder &b, const std::string &f): Header(b, f) { }
	virtual const char *get_type() const { return "SystemHeader"; }
};

#endif
