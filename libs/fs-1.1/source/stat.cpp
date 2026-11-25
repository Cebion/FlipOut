/* $Id$

This file is part of libmspfs
Copyright © 2006-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cerrno>
#ifdef WIN32
#include <io.h>
#endif
#include <msp/core/except.h>
#include "path.h"
#include "stat.h"

namespace Msp {
namespace FS {

int stat(const Path &fn, struct stat &st)
{
	return ::stat(fn.str().c_str(), &st);
}

struct stat stat(const Path &fn)
{
	struct stat st;
	if(stat(fn, st)==-1)
		throw SystemError("stat failed", errno);
	return st;
}

int lstat(const Path &fn, struct stat &st)
{
#ifdef WIN32
	return stat(fn, st);
#else
	return ::lstat(fn.str().c_str(), &st);
#endif
}

struct stat lstat(const Path &fn)
{
	struct stat st;
	if(lstat(fn, st)==-1)
		throw SystemError("lstat failed", errno);
	return st;
}

bool exists(const Path &path)
{
	return access(path.str().c_str(), F_OK)==0;
}

bool is_reg(const Path &path)
{
	struct stat st;
	if(stat(path, st)==0)
		return S_ISREG(st.st_mode);
	return false;
}

bool is_dir(const Path &path)
{
	struct stat st;
	if(stat(path, st)==0)
		return S_ISDIR(st.st_mode);
	return false;
}

bool is_link(const Path &path)
{
#ifdef WIN32
	(void)path;
#else
	struct stat st;
	if(lstat(path, st)==0)
		return S_ISLNK(st.st_mode);
#endif
	return false;
}

} // namespace FS
} // namespace Msp
