/* $Id: thread.h 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore
Copyright © 2006 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_CORE_THREAD_H_
#define MSP_CORE_THREAD_H_

#include "types.h"

namespace Msp {

/**
Base class for threads.  To create a thread for some task, derive it from this
class and implement the main() function.  Note that threads are not
automatically started upon creation - you must manually call launch() instead.
This is to allow initializing variables of the derived class before the thread
is started.
*/
class Thread
{
private:
	ThreadHandle thread_;
	bool         launched_;

protected:
	Thread(): launched_(false) { }
public:
	virtual ~Thread();

	void join();
	void cancel();
	void kill();
protected:
	void launch();
	virtual void main()=0;
	void check_cancel();

private:
	static
#ifdef WIN32
	DWORD WINAPI
#else
	void *
#endif
	main_(void *t) { (reinterpret_cast<Thread *>(t))->main(); return 0; }

	Thread(const Thread &);
	Thread &operator=(const Thread &);
};

} // namespace Msp

#endif
