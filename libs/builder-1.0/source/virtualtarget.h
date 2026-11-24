/* $Id: virtualtarget.h 93 2009-08-26 11:16:55Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef VIRTUALTARGET_H_
#define VIRTUALTARGET_H_

#include "target.h"

/**
A target that is not associated with any file.
*/
class VirtualTarget: public Target
{
public:
	VirtualTarget(Builder &b, const std::string &n): Target(b, 0, n) { }
	virtual const char *get_type() const { return "VirtualTarget"; }
private:
	virtual void check_rebuild();
	virtual Action *create_action() { return 0; }
};

#endif
