/* $Id: virtualtarget.cpp 93 2009-08-26 11:16:55Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/fs/path.h>
#include <msp/fs/utils.h>
#include "virtualtarget.h"

using namespace std;
using namespace Msp;

void VirtualTarget::check_rebuild()
{
	// Virtual targets are only rebuilt if their dependencies need rebuilding.
	for(TargetList::iterator i=depends.begin(); (i!=depends.end() && !rebuild); ++i)
		if((*i)->get_rebuild())
			mark_rebuild(FS::basename((*i)->get_name())+" needs rebuilding");
}
