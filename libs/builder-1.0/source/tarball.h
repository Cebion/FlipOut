/* $Id: tarball.h 90 2009-05-11 09:51:01Z tdb $

This file is part of builder
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef TARBALL_H_
#define TARBALL_H_

#include "filetarget.h"

class TarBall: public FileTarget
{
public:
	TarBall(Builder &, const SourcePackage &, const std::string &);
	virtual const char *get_type() const { return "TarBall"; }
	const SourcePackage *get_package() const;
private:
	virtual Action *create_action();
};

#endif
