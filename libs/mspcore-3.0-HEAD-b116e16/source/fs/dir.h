#ifndef MSP_FS_DIR_H_
#define MSP_FS_DIR_H_

#include <stdexcept>
#include <string>
#include <vector>
#include "path.h"

namespace Msp {
namespace FS {

class not_a_directory: public std::runtime_error
{
public:
	not_a_directory(const Path &);
	virtual ~not_a_directory() throw() { }
};

/// Creates a directory
void mkdir(const Path &path, int mode);

/// Creates a directory and any required parent directories
void mkpath(const Path &path, int mode);

/// Removes a directory, which must be empty
void rmdir(const Path &path);

/// Removes a directory and anything it contains
void rmpath(const Path &path);

/// Lists the contents of a directory
std::vector<std::string> list_files(const Path &path);

/// Lists the contents of a directory, filtered with a regex
std::vector<std::string> list_filtered(const Path &path, const std::string &filter);

/// Returns the current working directory
Path getcwd();

/// Changes the current working directory
void chdir(const Path &);

/// Returns the user's home directory
Path get_home_dir();

/// Returns a directory suitable for storing user-specific data.
Path get_user_data_dir();

/// Returns a directory containing system-wide configuration.
Path get_sys_conf_dir();

/// Returns a directory containing immutable system-wide data.
Path get_sys_data_dir();

/// Returns a directory containing system-wide architecture-specific files.
Path get_sys_lib_dir();

/** Looks for a file in a list of paths.  Returns the absolute path to the
first existing location, or an empty Path if the file is not found at all. */
Path path_lookup(const std::string &, const std::vector<Path> &);

/** Looks for a file using the PATH environment variable. */
Path path_lookup(const std::string &);

} // namespace FS
} // namespace Msp

#endif
