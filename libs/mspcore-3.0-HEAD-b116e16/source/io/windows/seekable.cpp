#include <windows.h>
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
		return FILE_BEGIN;
	else if(st==S_CUR)
		return FILE_CURRENT;
	else if(st==S_END)
		return FILE_END;

	throw invalid_argument("sys_seek_type");
}

}

namespace Msp {
namespace IO {

SeekOffset sys_seek(Handle &handle, SeekOffset offset, SeekType type)
{
	LONG high = offset>>32;
	DWORD ret = SetFilePointer(*handle, offset, &high, sys_seek_type(type));
	if(ret==INVALID_SET_FILE_POINTER)
	{
		DWORD err = GetLastError();
		if(err!=NO_ERROR)
			throw system_error("SetFilePointer");
	}

	return (SeekOffset(high)<<32) | ret;
}

} // namespace IO
} // namespace Msp
