/* $Id: unlink.h 85 2009-02-21 00:45:50Z tdb $

This file is part of builder
Copyright © 2007, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef UNLINK_H_
#define UNLINK_H_

#include "action.h"

class FileTarget;

class Unlink: public Action
{
public:
	Unlink(Builder &, const FileTarget &);
	virtual int check();
};

#endif
