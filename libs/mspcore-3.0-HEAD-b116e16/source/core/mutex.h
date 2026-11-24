#ifndef MSP_CORE_MUTEX_H_
#define MSP_CORE_MUTEX_H_

#include "noncopyable.h"
#include "refptr.h"

namespace Msp {

/**
A class for controlling mutually exclusive access to a resource.  Only one
thread can hold a lock on the mutex at a time.
*/
class Mutex: private NonCopyable
{
	friend class Semaphore;

private:
	struct Private;

	Private *priv;

public:
	Mutex();
	~Mutex();

	/** Locks the mutex.  If the mutex is already locked, waits until it becomes
	available. */
	void lock();

	/** Attempts to lock the mutex.  Returns true if the lock was acquired,
	otherwise returns false. */
	bool trylock();

	/** Unlocks the mutex. */
	void unlock();
};

/**
Locks the mutex for the lifetime of the object.
*/
class MutexLock
{
private:
	Mutex &mutex;

	MutexLock(const MutexLock &);
public:
	MutexLock(Mutex &m, bool l = true): mutex(m) { if(l) mutex.lock(); }
	~MutexLock() { mutex.unlock(); }

	void lock() { mutex.lock(); }
};

/**
Protects a pointer with a mutex.  As long as the MutexPtr (or a copy of it)
exists, the mutex will stay locked.
*/
template<typename T>
class MutexPtr
{
private:
	RefPtr<MutexLock> mutex;
	T *data;

public:
	MutexPtr(T *d, Mutex &m): mutex(new MutexLock(m)), data(d) { }

	T &operator*() const { return *data; }
	T *operator->() const { return data; }
	void clear() { mutex = 0; data = 0; }
};

}

#endif
