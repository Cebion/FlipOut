#include <pthread.h>
#include <signal.h>
#include "thread.h"
#include "thread_private.h"

namespace Msp {

void Thread::platform_join()
{
	pthread_join(priv_->handle, 0);
}

void Thread::platform_kill()
{
	pthread_kill(priv_->handle, SIGKILL);
}

void Thread::platform_launch()
{
	pthread_create(&priv_->handle, 0, &Private::main_wrapper, this);
}

void Thread::platform_setname()
{
#if defined(__GLIBC__) && (__GLIBC__>2 || (__GLIBC__==2 && __GLIBC_MINOR__>=12))
	if(!name_.empty())
		pthread_setname_np(priv_->handle, name_.c_str());
#endif
}

} // namespace Msp
