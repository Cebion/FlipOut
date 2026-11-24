#ifndef MSP_CORE_PROCESS_PRIVATE_H_
#define MSP_CORE_PROCESS_PRIVATE_H_

#include "process_platform.h"

namespace Msp {

struct Process::Private
{
	PlatformProcessInformation info;

	Private();
};

} // namespace Msp

#endif
