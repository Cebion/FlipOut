/* $Id: archive.h 43 2007-08-15 09:48:30Z tdb $

This file is part of builder
Copyright © 2006-2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef ARCHIVE_H_
#define ARCHIVE_H_

#include "externalaction.h"

class StaticLibrary;

/**
Creates an archive of object files, a.k.a. static library.
*/
class Archive: public ExternalAction
{
public:
	Archive(Builder &, const StaticLibrary &);
};

#endif
