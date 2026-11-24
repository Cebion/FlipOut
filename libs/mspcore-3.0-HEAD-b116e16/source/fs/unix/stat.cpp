#define _FILE_OFFSET_BITS 64
#include <cerrno>
#include <unistd.h>
#include <sys/stat.h>
#include <msp/core/systemerror.h>
#include <msp/strings/format.h>
#include "stat.h"
#include "stat_private.h"

namespace Msp {
namespace FS {

Stat::Private::Private(const Private &other):
	owner_id(other.owner_id),
	group_id(other.group_id)
{ }

Stat::Private::~Private()
{ }

Stat Stat::Private::from_struct_stat(const struct stat &st)
{
	Stat result;
	result.exists = true;
	if(S_ISREG(st.st_mode))
		result.type = REGULAR;
	else if(S_ISDIR(st.st_mode))
		result.type = DIRECTORY;
	else if(S_ISLNK(st.st_mode))
		result.type = SYMLINK;
	else
		result.type = UNKNOWN;
	result.size = st.st_size;
	result.alloc_size = st.st_blocks*512;
	result.mtime = Time::TimeStamp::from_unixtime(st.st_mtime);

	result.priv = new Private;
	result.priv->owner_id = st.st_uid;
	result.priv->group_id = st.st_gid;

	return result;
}


Stat Stat::stat(const Path &path)
{
	struct stat st;
	int ret = ::stat(path.str().c_str(), &st);
	if(ret==-1)
	{
		int err = errno;
		if(err==ENOENT)
			return Stat();
		else
			throw system_error(format("stat(%s)", path), err);
	}

	return Private::from_struct_stat(st);
}

Stat Stat::lstat(const Path &path)
{
	struct stat st;
	int ret = ::lstat(path.str().c_str(), &st);
	if(ret==-1)
	{
		int err = errno;
		if(err==ENOENT)
			return Stat();
		else
			throw system_error(format("lstat(%s)", path), err);
	}

	return Private::from_struct_stat(st);
}

bool exists(const Path &path)
{
	return access(path.str().c_str(), F_OK)==0;
}

} // namespace FS
} // namespace Msp
