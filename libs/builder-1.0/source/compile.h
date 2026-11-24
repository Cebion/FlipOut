/* $Id: compile.h 84 2009-02-19 16:43:37Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef COMPILE_H_
#define COMPILE_H_

#include <msp/fs/path.h>
#include "externalaction.h"

class Component;
class ObjectFile;

/**
Compiles a source file into an object file.
*/
class Compile: public ExternalAction
{
public:
	Compile(Builder &, const ObjectFile &);
};

#endif
