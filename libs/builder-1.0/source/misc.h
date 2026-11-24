/* $Id: misc.h 91 2009-05-11 20:43:21Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MISC_H_
#define MISC_H_

#include <list>
#include <map>
#include <string>
#include <msp/fs/path.h>

enum LibMode
{
	ALL_STATIC,
	LOCAL_STATIC,
	DYNAMIC
};

typedef std::list<std::string> StringList;
typedef std::list<Msp::FS::Path> PathList;
typedef std::map<std::string, std::string> StringMap;

/** Runs a command and returns its output as a string.  The exit status of the
command is returned in the second parameter if it is not null. */
std::string run_command(const StringList &, int * =0);

#endif
