#include <windows.h>
#include <msp/core/systemerror.h>
#include <msp/time/timedelta.h>
#include "semaphore.h"

namespace Msp {

struct Semaphore::Private
{
	HANDLE handle;
};


Semaphore::Semaphore(unsigned limit):
	priv(new Private)
{
	priv->handle = CreateSemaphore(0, 0, limit, 0);
}

Semaphore::~Semaphore()
{
	CloseHandle(priv->handle);
	delete priv;
}

void Semaphore::signal()
{
	if(!ReleaseSemaphore(priv->handle, 1, 0))
		throw system_error("ReleaseSemaphore");
}

void Semaphore::wait()
{ 
	DWORD ret = WaitForSingleObject(priv->handle, INFINITE);
	if(ret==WAIT_FAILED)
		throw system_error("WaitForSingleObject");
}

bool Semaphore::wait(const Time::TimeDelta &d)
{
	DWORD ret = WaitForSingleObject(priv->handle, static_cast<DWORD>(d/Time::msec));
	if(ret==WAIT_FAILED)
		throw system_error("WaitForSingleObject");
	return ret==WAIT_OBJECT_0;
}

} // namespace Msp
