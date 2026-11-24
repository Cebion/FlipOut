/* $Id: mutex.h 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore
Copyright © 2006 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_CORE_MUTEX_H_
#define MSP_CORE_MUTEX_H_

#include "refptr.h"
#include "types.h"

namespace Msp {

class Mutex
{
	friend class Semaphore;

private:
	MutexHandle mutex;

public:
#ifndef WIN32
	Mutex()       { pthread_mutex_init(&mutex, 0); }
	int lock()    { return pthread_mutex_lock(&mutex); }
	int trylock() { return pthread_mutex_trylock(&mutex); }
	int unlock()  { return pthread_mutex_unlock(&mutex); }
	~Mutex()      { pthread_mutex_destroy(&mutex); }
#else
	Mutex()       { mutex=CreateMutex(0, false, 0); }
	int lock()    { return WaitForSingleObject(mutex, INFINITE)==WAIT_OBJECT_0; }
	int trylock() { return WaitForSingleObject(mutex, 0)==WAIT_OBJECT_0; }
	int unlock()  { return !ReleaseMutex(mutex); }
	~Mutex()      { CloseHandle(mutex); }
#endif
};

/**
Locks the mutex for te lifetime of the object.
*/
class MutexLock
{
private:
	Mutex &mutex;

public:
	MutexLock(Mutex &m, bool l=true): mutex(m) { if(l) mutex.lock(); }
	~MutexLock() { mutex.unlock(); }

	int lock() { return mutex.lock(); }
private:
	MutexLock(const MutexLock &);
	MutexLock &operator=(const MutexLock &);
};

/**
Protects a pointer with a mutex.  As long as the MutexPtr (or a copy of it)
exists, the mutex will stay locked.
*/
template<typename T>
class MutexPtr
{
public:
	MutexPtr(T *d, Mutex &m): mutex(new MutexLock(m)), data(d) { }

	T &operator*() const { return *data; }
	T *operator->() const { return data; }
	void clear() { mutex=0; data=0; }
private:
	RefPtr<MutexLock> mutex;
	T *data;
};

/*template<typename T>
class MutexPtr: public RefCount
{
public:
	MutexPtr(T *d, Mutex &m): mutex(m), data(d) { mutex.lock(); }
	MutexPtr(const MutexPtr<T> &p): RefCount(p), mutex(p.mutex), data(p.data) { }
	T &operator*() const { return *data; }
	T *operator->() const { return data; }
	void clear() { decref(); data=0; }
	~MutexPtr() { decref(); }
protected:
	Mutex &mutex;
	T     *data;

	bool decref()
	{
		if(!RefCount::decref())
		{
			mutex.unlock();
			return false;
		}
		return true;
	}
};*/

}

#endif
