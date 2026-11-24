#ifndef MSP_CORE_THREAD_PRIVATE_H_
#define MSP_CORE_THREAD_PRIVATE_H_

#include "thread.h"
#include "thread_platform.h"

namespace Msp {

struct Thread::Private
{
	ThreadHandle handle;

	Private(): handle(0) { }

	static ThreadReturn THREAD_CALL main_wrapper(void *);
};

} // namespace Msp

#endif
