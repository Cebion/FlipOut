#ifndef MSP_CORE_PROCESS_PLATFORM_H_
#define MSP_CORE_PROCESS_PLATFORM_H_

#include <sys/types.h>

namespace Msp {

struct PlatformProcessInformation
{
	pid_t pid;
};

} // namespace Msp

#endif
