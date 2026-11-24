#ifndef MSP_CORE_MUTEX_PLATFORM_H_
#define MSP_CORE_MUTEX_PLATFORM_H_

#include <windows.h>

namespace Msp {

typedef CRITICAL_SECTION PlatformMutex;

} // namespace Msp

#endif
