#ifndef MSP_CORE_MUTEX_PRIVATE_H_
#define MSP_CORE_MUTEX_PRIVATE_H_

#include "mutex.h"
#include "mutex_platform.h"

namespace Msp {

struct Mutex::Private
{
	PlatformMutex mutex;
};

} // namespace Msp

#endif
