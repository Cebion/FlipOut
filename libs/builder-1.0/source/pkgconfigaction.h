/* $Id: pkgconfigaction.h 84 2009-02-19 16:43:37Z tdb $

This file is part of builder
Copyright © 2006-2007, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef PKGCONFIGACTION_H_
#define PKGCONFIGACTION_H_

#include "action.h"

class PkgConfig;

/**
Action for building a PkgConfig target.  I just couldn't come up with a less
lame name...
*/
class PkgConfigAction: public Action
{
public:
	PkgConfigAction(Builder &, const PkgConfig &);
	virtual int check();
};

#endif
