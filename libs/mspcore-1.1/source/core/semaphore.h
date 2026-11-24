/* $Id: semaphore.h 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_CORE_SEMAPHORE_H_
#define MSP_CORE_SEMAPHORE_H_

#include "mutex.h"
#include "types.h"
#include "../time/timedelta.h"

namespace Msp {

class Semaphore
{
private:
	Mutex *mutex;
	bool  own_mutex;
	SemaphoreHandle sem;
#ifdef WIN32
	unsigned count;
#endif

public:
	Semaphore();
	Semaphore(Mutex &);
private:
	void init();
public:
	~Semaphore();

	int   signal();
	int   broadcast();
	int   wait();
	int   wait(const Time::TimeDelta &);
	Mutex &get_mutex() { return *mutex; }
};

#ifndef WIN32
inline int Semaphore::signal()
{ return pthread_cond_signal(&sem); }

inline int Semaphore::broadcast()
{ return pthread_cond_broadcast(&sem); }

inline int Semaphore::wait()
{ return pthread_cond_wait(&sem, &mutex->mutex); }
#endif

}

#endif
