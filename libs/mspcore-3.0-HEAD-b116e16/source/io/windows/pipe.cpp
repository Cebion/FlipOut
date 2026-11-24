#include <msp/core/systemerror.h>
#include <msp/strings/format.h>
#include "handle_private.h"
#include "pipe.h"

using namespace std;

namespace Msp {
namespace IO {

void Pipe::platform_init()
{
	string name = format("\\\\.\\pipe\\%u.%p", GetCurrentProcessId(), this);
	*read_handle = CreateNamedPipe(name.c_str(), PIPE_ACCESS_INBOUND|FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE, 1, 1024, 1024, 0, 0);
	if(!read_handle)
		throw system_error("CreateNamedPipe");

	*write_handle = CreateFile(name.c_str(), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
	if(!write_handle)
	{
		unsigned err = GetLastError();
		CloseHandle(*read_handle);
		throw system_error(format("CreateFile(%s)", name), err);
	}
}

} // namespace IO
} // namespace Msp
