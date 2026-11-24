#include <windows.h>
#include "rawtime_private.h"
#include "timedelta.h"
#include "timestamp.h"
#include "utils.h"

namespace Msp {
namespace Time {

TimeStamp now()
{
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return TimeStamp(filetime_to_rawtime(ft));
}

TimeDelta get_cpu_time()
{
	//XXX Figure out the function to use on Win32
	return TimeDelta();
}

void sleep(const TimeDelta &d)
{
	Sleep((DWORD)(d/msec));
}

} // namespace Time
} // namespace Msp
