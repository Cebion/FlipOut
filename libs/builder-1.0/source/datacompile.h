/* $Id: datacompile.h 98 2009-09-20 17:34:22Z tdb $

This file is part of builder
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef DATACOMPILE_H_
#define DATACOMPILE_H_

#include "externalaction.h"

class DataFile;

class DataCompile: public ExternalAction
{
public:
	DataCompile(Builder &, DataFile &);
};

#endif
