#define _FILE_OFFSET_BITS 64
#include <cerrno>
#include <unistd.h>
#include <msp/core/systemerror.h>
#include "handle.h"
#include "handle_private.h"
#include "seekable.h"

using namespace std;

namespace {

using namespace Msp::IO;

int sys_seek_type(SeekType st)
{
	if(st==S_BEG)
		return SEEK_SET;
	else if(st==S_CUR)
		return SEEK_CUR;
	else if(st==S_END)
		return SEEK_END;

	throw invalid_argument("sys_seek_type");
}

}

namespace Msp {
namespace IO {

/* Android libc does not recognize _FILE_OFFSET_BITS so this hack is necessary
to get 64-bit seeks. */
#ifdef __ANDROID__
#define off_t off64_t
#define lseek lseek64
#endif

SeekOffset sys_seek(Handle &handle, SeekOffset offset, SeekType type)
{
	off_t ret = lseek(*handle, offset, sys_seek_type(type));
	if(ret==(off_t)-1)
	{
		if(errno==EINVAL)
			throw bad_seek(offset, type);
		else
			throw system_error("lseek64");
	}

	return ret;
}

} // namespace IO
} // namespace Msp
