#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <msp/core/systemerror.h>
#include <msp/strings/format.h>
#include "file.h"
#include "handle_private.h"

using namespace std;

namespace Msp {
namespace IO {

void File::platform_init(const string &fn, CreateMode cm)
{
	int flags = 0;
	switch(mode&M_RDWR)
	{
	case M_READ:  flags |= O_RDONLY; break;
	case M_WRITE: flags |= O_WRONLY; break;
	case M_RDWR:  flags |= O_RDWR; break;
	default:;
	}

	if(mode&M_WRITE)
	{
		if(cm&C_CREATE)
			flags |= O_CREAT;
		if(cm&C_TRUNCATE)
			flags |= O_TRUNC;
		if(cm&C_EXCLUSIVE)
			flags |= O_EXCL;
	}
	if(mode&M_APPEND)
		flags |= O_APPEND;
	if(mode&M_NONBLOCK)
		flags |= O_NONBLOCK;
	if(!(mode&M_INHERIT))
		flags |= O_CLOEXEC;

	*handle = ::open(fn.c_str(), flags, 0666);
	if(!handle)
	{
		int err = errno;
		if(err==ENOENT)
			throw file_not_found(fn);
		else if(err==EEXIST)
			throw file_already_exists(fn);
		else
			throw system_error(format("open(%s)", fn), err);
	}
}

void File::sync()
{
	signal_flush_required.emit();

	fsync(*handle);
}

} // namespace IO
} // namespace Msp
