/* $Id: filetarget.cpp 102 2009-09-21 16:23:33Z tdb $

This file is part of builder
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/fs/stat.h>
#include <msp/fs/utils.h>
#include "file.h"

using namespace std;
using namespace Msp;

FileTarget::FileTarget(Builder &b, const Package *p, const FS::Path &a):
	// XXX Builder depends on target name being its path for locating file targets
	Target(b, p, /*FS::basename*/(a.str())),
	path(a),
	size(0)
{
	struct stat st;
	if(!FS::stat(path, st))
	{
		mtime=Time::TimeStamp::from_unixtime(st.st_mtime);
		size=st.st_size;
	}
}
