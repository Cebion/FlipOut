/* $Id$

This file is part of libmspfs
Copyright © 2006-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_FS_STAT_H_
#define MSP_FS_STAT_H_

#include <sys/stat.h>
#include "path.h"

namespace Msp {
namespace FS {

/**
Gets information about a file.  Returns 0 on success or -1 on error.  This
version can be used to check for file existence and get information in one
call.
*/
int stat(const Path &fn, struct stat &st);

/**
Returns information about a file.  This version throws an exception if an error
occurs.
*/
struct stat stat(const Path &fn);

/// Gets information about a file, without following symbolic links
int lstat(const Path &fn, struct stat &st);

/// Returns information about a file, without following symbolic links
struct stat lstat(const Path &fn);

/// Tests for existence of a file
bool exists(const Path &path);

/// Tests whether a path refers to an existing regular file
bool is_reg(const Path &path);

/// Tests whether a path refers to an existing directory
bool is_dir(const Path &path);

/// Tests whether a path refers to a symbolic link
bool is_link(const Path &path);

} // namespace FS
} // namespace Msp

#endif
