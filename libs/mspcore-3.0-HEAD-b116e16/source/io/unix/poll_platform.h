#ifndef MSP_IO_POLL_PLATFORM_H_
#define MSP_IO_POLL_PLATFORM_H_

#include <poll.h>
#include <vector>

namespace Msp {
namespace IO {

struct Poller::Private
{
	std::vector<pollfd> pfd;
};

} // namespace IO
} // namespace Msp

#endif
