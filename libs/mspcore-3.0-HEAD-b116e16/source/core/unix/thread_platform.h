#ifndef MSP_CORE_THREAD_PLATFORM_H_
#define MSP_CORE_THREAD_PLATFORM_H_

#include <pthread.h>

namespace Msp {

typedef pthread_t ThreadHandle;
typedef void *ThreadReturn;

#define THREAD_CALL

} // namespace Msp

#endif
