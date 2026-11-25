/* $Id$

This file is part of libmspfs
Copyright © 2006-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_FS_UTILS_H_
#define MSP_FS_UTILS_H_

#include "path.h"

namespace Msp {
namespace FS {

/// Extracts the last component of the path.
std::string basename(const Path &);

/// Removes the last component from the path.
Path dirname(const Path &);

/**
Returns the base part of a filename.  This includes everything up to the last
dot, but not the dot itself.
*/
std::string basepart(const std::string &);

/**
Returns the extension part of a filename.  This includes the last dot and
everything after it.
*/
std::string extpart(const std::string &);

/**
Fixes the case of the given path to match files / directories on the
filesystem.
*/
Path fix_case(const Path &path);

/// Reads the contents of a symbolic link
Path readlink(const Path &path);

/// Resolves all symlinks from a path.  Will always return an absolute path.
Path realpath(const Path &path);

/// Removes a file
void unlink(const Path &path);

/// Makes a path relative to some base path.  That is, base/result==path.
Path relative(const Path &path, const Path &base);

} // namespace FS
} // namespace Msp

#endif
