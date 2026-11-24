#ifndef MSP_IO_POLL_PLATFORM_H_
#define MSP_IO_POLL_PLATFORM_H_

#include <windows.h>
#include <vector>

namespace Msp {
namespace IO {

struct Poller::Private
{
	std::vector<HANDLE> handles;
};

} // namespace IO
} // namespace Msp

#endif
