#ifndef MSP_IO_HANDLE_PRIVATE_H_
#define MSP_IO_HANDLE_PRIVATE_H_

#include "handle.h"
#include "handle_platform.h"

namespace Msp {
namespace IO {

struct Handle::Private
{
	PlatformHandle handle;

	Private();

	Private &operator=(PlatformHandle);
	operator PlatformHandle() const { return handle; }
};

} // namespace IO
} // namespace Msp

#endif
