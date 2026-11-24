/* $Id: types.h 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_CORE_TYPES_H_
#define MSP_CORE_TYPES_H_

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

namespace Msp {

#ifdef WIN32
typedef HANDLE ThreadHandle;
typedef HANDLE MutexHandle;
typedef HANDLE SemaphoreHandle;
#else
typedef pthread_t ThreadHandle;
typedef pthread_mutex_t MutexHandle;
typedef pthread_cond_t SemaphoreHandle;
#endif

} // namespace Msp

#endif
