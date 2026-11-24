#ifndef MSP_CORE_MUTEX_PLATFORM_H_
#define MSP_CORE_MUTEX_PLATFORM_H_

#include <pthread.h>

namespace Msp {

typedef pthread_mutex_t PlatformMutex;

} // namespace Msp

#endif
