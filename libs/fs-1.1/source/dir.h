/* $Id$

This file is part of libmspfs
Copyright © 2006-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_FS_DIR_H_
#define MSP_FS_DIR_H_

#include <list>
#include <string>
#include "path.h"

namespace Msp {
namespace FS {

/// Creates a directory
void mkdir(const Path &path, int mode);

/// Creates a directory and any required parent directories
void mkpath(const Path &path, int mode);

/// Removes a directory, which must be empty
void rmdir(const Path &path);

/// Removes a directory and anything it contains
void rmpath(const Path &path);

/// Lists the contents of a directory
std::list<std::string> list_files(const Path &path);

/// Lists the contents of a directory, filtered with a regex
std::list<std::string> list_filtered(const Path &path, const std::string &filter);

/// Returns the current working directory
Path getcwd();

/// Returns the user's home directory
Path get_home_dir();

/// Returns a directory suitable for storing user-specific data
Path get_user_data_dir(const std::string &appname);

/// Returns a directory containing system-wide configuration
Path get_sys_conf_dir(const std::string &argv0);

/// Returns a directory containing immutable system-wide data
Path get_sys_data_dir(const std::string &argv0, const std::string &appname);

/// Changes the current working directory
void chdir(const Path &);

} // namespace FS
} // namespace Msp

#endif
