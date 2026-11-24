/* $Id: file.h 96 2009-09-11 22:48:06Z tdb $

This file is part of builder
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef FILE_H_
#define FILE_H_

#include "filetarget.h"

/**
Just an arbitary file.  No special meaning attached.
*/
class File: public FileTarget
{
public:
	File(Builder &b, const Msp::FS::Path &t): FileTarget(b, 0, t) { }
	File(Builder &b, Package &p, const Msp::FS::Path &t): FileTarget(b, &p, t) { }
	virtual const char *get_type() const { return "File"; }
private:
	virtual Action *create_action() { return 0; }
};

#endif
