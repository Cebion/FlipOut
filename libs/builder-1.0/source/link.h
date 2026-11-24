/* $Id: link.h 85 2009-02-21 00:45:50Z tdb $

This file is part of builder
Copyright © 2006-2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef LINK_H_
#define LINK_H_

#include "externalaction.h"

class Executable;

/**
Links object files and libraries to produce an executable.
*/
class Link: public ExternalAction
{
public:
	Link(Builder &, const Binary &);
};

#endif
