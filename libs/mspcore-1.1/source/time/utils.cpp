/* $Id: utils.cpp 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore     
Copyright © 2006-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifdef WIN32
#include <windows.h>
#else
#include <sys/resource.h>
#include <sys/time.h>
#endif
#include "datetime.h"
#include "timedelta.h"
#include "timestamp.h"
#include "units.h"
#include "utils.h"

using namespace std;

namespace Msp {
namespace Time {

/**
Returns the current timestamp.
*/
TimeStamp now()
{
#ifndef WIN32
	timeval tv;
	gettimeofday(&tv, 0);
	return TimeStamp(tv.tv_sec*1000000LL+tv.tv_usec);
#else
	static RawTime epoch=0;
	if(!epoch)
	{
		SYSTEMTIME st;
		st.wYear=1970;
		st.wMonth=1;
		st.wDay=1;
		st.wHour=0;
		st.wMinute=0;
		st.wSecond=0;
		st.wMilliseconds=0;

		FILETIME ft;
		SystemTimeToFileTime(&st, &ft);
		epoch=(ft.dwLowDateTime+(static_cast<RawTime>(ft.dwHighDateTime)<<32))/10;
	}
	
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return TimeStamp((ft.dwLowDateTime+(static_cast<RawTime>(ft.dwHighDateTime)<<32))/10-epoch);
#endif
}

string format_now(const string &fmt)
{
	return DateTime(now()).format(fmt);
}

/**
Returns the CPU time used by the program so far.
*/
TimeDelta get_cpu_time()
{
#ifndef WIN32
	rusage ru;
	getrusage(RUSAGE_SELF, &ru);
	return (ru.ru_utime.tv_sec+ru.ru_stime.tv_sec)*sec + (ru.ru_utime.tv_usec+ru.ru_stime.tv_usec)*usec;
#else
	//XXX Figure out the function to use on Win32
	return TimeDelta();
#endif
}

/**
Sleeps for the given time.
*/
int sleep(const TimeDelta &d)
{
#ifndef WIN32
	timespec ts;
	d.fill_timespec(ts);
	return nanosleep(&ts, 0);
#else
	Sleep((DWORD)(d/msec));
	return 0;
#endif
}

} // namespace Time
} // namespace Msp
