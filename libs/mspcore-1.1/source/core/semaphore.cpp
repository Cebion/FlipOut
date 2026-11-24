/* $Id: semaphore.cpp 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef WIN32
#include <sys/time.h>
#endif
#include <errno.h>
#include "semaphore.h"
#include "../time/timestamp.h"
#include "../time/units.h"
#include "../time/utils.h"

namespace Msp {

Semaphore::Semaphore():
	mutex(new Mutex),
	own_mutex(true)
{
	init();
}

Semaphore::Semaphore(Mutex &m):
	mutex(&m),
	own_mutex(false)
{
	init();
}

void Semaphore::init()
{
#ifdef WIN32
	count=0;
	sem=CreateSemaphore(0, 0, 32, 0);
#else
	pthread_cond_init(&sem, 0);
#endif
}

Semaphore::~Semaphore()
{
	if(own_mutex)
		delete mutex;
#ifdef WIN32
	CloseHandle(sem);
#else
	pthread_cond_destroy(&sem);
#endif
}

#ifdef WIN32
int Semaphore::signal()
{
	if(count==0) 
		return 0;

	int ret=!ReleaseSemaphore(sem, 1, 0); 

	unsigned old_count=count;
	mutex->unlock();
	while(count==old_count)
		Sleep(0);
	mutex->lock();

	return ret;
}

int Semaphore::broadcast()
{
	if(count==0)
		return 0;
	int ret=!ReleaseSemaphore(sem, count, 0);

	mutex->unlock();
	while(count)
		Sleep(0);
	mutex->lock();

	return ret;
}

int Semaphore::wait()
{ 
	++count; 
	mutex->unlock();
	DWORD ret=WaitForSingleObject(sem, INFINITE); 
	mutex->lock();
	--count;
	
	return ret==WAIT_OBJECT_0;
}
#endif

int Semaphore::wait(const Time::TimeDelta &d)
{
#ifndef WIN32
	Time::TimeStamp ts=Time::now()+d;

	timespec timeout;
	timeout.tv_sec=ts.raw()/1000000;
	timeout.tv_nsec=(ts.raw()%1000000)*1000;

	int r=pthread_cond_timedwait(&sem, &mutex->mutex, &timeout);
	if(r==ETIMEDOUT)
		return 1;
	else if(r)
		return -1;
	return 0;
#else
	++count;
	mutex->lock();
	DWORD ret=WaitForSingleObject(sem, (DWORD)(d/Time::usec));
	mutex->unlock();
	--count;
	return ret==WAIT_OBJECT_0;
#endif
}

} // namespace Msp
