#include <windows.h>
#include "thread.h"
#include "thread_private.h"

namespace Msp {

void Thread::platform_join()
{
	WaitForSingleObject(priv_->handle, INFINITE);
}

void Thread::platform_kill()
{
	TerminateThread(priv_->handle, 0);
}

void Thread::platform_launch()
{
	DWORD dummy;  // Win9x needs the lpTthreadId parameter
	priv_->handle = CreateThread(0, 0, &Private::main_wrapper, this, 0, &dummy);
}

void Thread::platform_setname()
{
	// TODO: https://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
}

} // namespace Msp
