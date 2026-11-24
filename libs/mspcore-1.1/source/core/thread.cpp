/* $Id: thread.cpp 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore
Copyright © 2006 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef WIN32
#include <signal.h>
#endif
#include "thread.h"

namespace Msp {

/**
Waits for the thread to exit.  Calling this from the thread will cause a
deadlock.
*/
void Thread::join()
{
	if(!launched_)
		return;

#ifdef WIN32
	WaitForSingleObject(thread_, INFINITE);
#else
	pthread_join(thread_, 0);
#endif
	launched_=false;
}

/**
Requests the thread to terminate gracefully.  Currently unimplemented on win32.
*/
void Thread::cancel()
{
#ifndef WIN32 //XXX
	pthread_cancel(thread_);
#endif
}

/**
Violently terminates the thread.
*/
void Thread::kill()
{
#ifdef WIN32
	TerminateThread(thread_, 0);
#else
	pthread_kill(thread_, SIGKILL);
#endif
}

Thread::~Thread()
{
	if(launched_)
		kill();
}

void Thread::launch()
{
	if(launched_)
		return;

#ifdef WIN32
	DWORD dummy;  // Win9x needs the lpTthreadId parameter
	thread_=CreateThread(0, 0, &main_, this, 0, &dummy);
#else
	pthread_create(&thread_, 0, &main_, this);
#endif
	launched_=true;
}

} // namespace Msp
