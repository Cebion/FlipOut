#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <msp/core/systemerror.h>
#include "handle.h"
#include "handle_private.h"

using namespace std;

namespace Msp {
namespace IO {

void sys_set_blocking(Handle &handle, bool b)
{
	int flags = fcntl(*handle, F_GETFL);
	fcntl(*handle, F_SETFL, (flags&~O_NONBLOCK)|(b?0:O_NONBLOCK));
}

void sys_set_inherit(Handle &handle, bool i)
{
	int flags = fcntl(*handle, F_GETFD);
	fcntl(*handle, F_SETFD, (flags&~O_CLOEXEC)|(i?O_CLOEXEC:0));
}

size_t sys_read(Handle &handle, char *buf, size_t size)
{
	ssize_t ret = read(*handle, buf, size);
	if(ret==-1)
	{
		if(errno==EAGAIN)
			return 0;
		else
			throw system_error("read");
	}

	return ret;
}

size_t sys_write(Handle &handle, const char *buf, size_t size)
{
	ssize_t ret = write(*handle, buf, size);
	if(ret==-1)
	{
		if(errno==EAGAIN)
			return 0;
		else
			throw system_error("write");
	}

	return ret;
}

void sys_close(Handle &handle)
{
	if(handle)
	{
		close(*handle);
		*handle = INVALID_HANDLE_VALUE;
	}
}

} // namespace IO
} // namespace Msp
