/* $Id: tarball.cpp 90 2009-05-11 09:51:01Z tdb $

This file is part of builder
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "sourcepackage.h"
#include "tar.h"
#include "tarball.h"

using namespace std;

TarBall::TarBall(Builder &b, const SourcePackage &p, const string &n):
	FileTarget(b, &p, p.get_source()/(n+".tar"))
{
	buildable=true;
}

const SourcePackage *TarBall::get_package() const
{
	return static_cast<const SourcePackage *>(package);
}

Action *TarBall::create_action()
{
	return new Tar(builder, *this);
}
