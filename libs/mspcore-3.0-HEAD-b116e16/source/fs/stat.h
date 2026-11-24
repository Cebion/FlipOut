#ifndef MSP_FS_STAT_H_
#define MSP_FS_STAT_H_

#include <cstdint>
#include <string>
#include <msp/time/timestamp.h>
#include "path.h"

namespace Msp {
namespace FS {

enum FileType
{
	UNKNOWN,
	REGULAR,
	DIRECTORY,
	SYMLINK
};

typedef uint64_t FileSize;

/**
Holds file information.
*/
class Stat
{
private:
	struct Private;

	struct OwnerInfo
	{
		std::string owner;
		std::string group;
	};

	bool exists;
	FileType type;
	FileSize size;
	FileSize alloc_size;
	Time::TimeStamp mtime;
	mutable OwnerInfo owner_info;
	Private *priv;

public:
	Stat();
	Stat(const Stat &);
	Stat &operator=(const Stat &);
	~Stat();

	FileType get_type() const { return type; }
	bool is_regular() const { return type==REGULAR; }
	bool is_directory() const { return type==DIRECTORY; }
	bool is_symlink() const { return type==SYMLINK; }
	FileSize get_size() const { return size; }
	FileSize get_alloc_size() const { return alloc_size; }
	const Time::TimeStamp &get_modify_time() const { return mtime; }
	const std::string &get_owner() const;
	const std::string &get_group() const;

	operator bool() const { return exists; }

	/// Returns a Stat object describing a file.
	static Stat stat(const Path &);
	static Stat lstat(const Path &);
};

/// Convenience wrapper for Stat::stat
inline Stat stat(const Path &path)
{ return Stat::stat(path); }

/// Convenience wrapper for Stat::lstat
inline Stat lstat(const Path &path)
{ return Stat::lstat(path); }

/// Tests for existence of a file
bool exists(const Path &path);

/// Tests whether a path refers to an existing regular file
inline bool is_reg(const Path &path)
{ return stat(path).is_regular(); }

/// Tests whether a path refers to an existing directory
inline bool is_dir(const Path &path)
{ return stat(path).is_directory(); }

/// Tests whether a path refers to a symbolic link
inline bool is_link(const Path &path)
{ return lstat(path).is_symlink(); }

} // namespace FS
} // namespace Msp

#endif
