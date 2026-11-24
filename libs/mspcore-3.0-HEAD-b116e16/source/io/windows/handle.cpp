#define NOMINMAX
#include <limits>
#include <msp/core/systemerror.h>
#include "handle.h"
#include "handle_private.h"

using namespace std;

namespace Msp {
namespace IO {

void sys_set_blocking(Handle &, bool)
{
}

void sys_set_inherit(Handle &, bool)
{
}

size_t sys_read(Handle &handle, char *buf, size_t size)
{
	if(size>numeric_limits<DWORD>::max())
		throw invalid_argument("read");

	DWORD ret;
	if(ReadFile(*handle, buf, size, &ret, 0)==0)
		throw system_error("ReadFile");

	return ret;
}

size_t sys_write(Handle &handle, const char *buf, size_t size)
{
	if(size>numeric_limits<DWORD>::max())
		throw invalid_argument("write");

	DWORD ret;
	if(WriteFile(*handle, buf, size, &ret, 0)==0)
		throw system_error("WriteFile");

	return ret;
}

void sys_close(Handle &handle)
{
	if(handle)
	{
		CloseHandle(*handle);
		*handle = INVALID_HANDLE_VALUE;
	}
}

} // namespace IO
} // namespace Msp
