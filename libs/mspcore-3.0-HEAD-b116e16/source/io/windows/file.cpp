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
	int share_flags = 0;
	int create_flags = OPEN_EXISTING;

	if(mode&M_READ)
		flags |= GENERIC_READ;

	if(mode&M_WRITE)
	{
		flags |= GENERIC_WRITE;

		switch(static_cast<int>(cm))
		{
		case C_NONE:     create_flags = OPEN_EXISTING; break;
		case C_CREATE:   create_flags = OPEN_ALWAYS; break;
		case C_TRUNCATE: create_flags = TRUNCATE_EXISTING; break;
		case C_OVERWRITE: create_flags = CREATE_ALWAYS; break;
		case C_NEW:      create_flags = CREATE_NEW; break;
		}
	}
	else
		share_flags = FILE_SHARE_READ;

	SECURITY_ATTRIBUTES sec_attr;
	sec_attr.nLength = sizeof(SECURITY_ATTRIBUTES);
	sec_attr.lpSecurityDescriptor = 0;
	sec_attr.bInheritHandle = !!(mode&M_INHERIT);

	*handle = CreateFile(fn.c_str(), flags, share_flags, 0, create_flags, FILE_ATTRIBUTE_NORMAL, &sec_attr);
	if(!handle)
	{
		int err = GetLastError();
		if(err==ERROR_FILE_NOT_FOUND || err==ERROR_PATH_NOT_FOUND)
			throw file_not_found(fn);
		else if(err==ERROR_FILE_EXISTS)
			throw file_already_exists(fn);
		else
			throw system_error(format("CreateFile(%s)", fn), err);
	}
}

void File::sync()
{
}

} // namespace IO
} // namespace Msp
