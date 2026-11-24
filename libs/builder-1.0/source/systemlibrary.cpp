/* $Id: systemlibrary.cpp 92 2009-05-13 20:15:38Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/fs/path.h>
#include <msp/fs/utils.h>
#include "systemlibrary.h"

using namespace std;
using namespace Msp;

SystemLibrary::SystemLibrary(Builder &b, const FS::Path &p):
	FileTarget(b, 0, p),
	Library(b, 0, p, extract_libname(p))
{ }

string SystemLibrary::extract_libname(const FS::Path &p)
{
	string result=FS::basepart(FS::basename(p));
	if(!result.compare(0, 3, "lib"))
		result.erase(0, 3);
	return result;
}
