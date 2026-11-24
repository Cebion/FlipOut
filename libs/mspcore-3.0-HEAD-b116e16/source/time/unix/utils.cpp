#include <sys/resource.h>
#include <sys/time.h>
#include <cerrno>
#include <msp/core/systemerror.h>
#include "rawtime_private.h"
#include "timedelta.h"
#include "timestamp.h"
#include "utils.h"

namespace Msp {
namespace Time {

TimeStamp now()
{
	timeval tv;
	gettimeofday(&tv, 0);
	return TimeStamp(timeval_to_rawtime(tv));
}

TimeDelta get_cpu_time()
{
	rusage ru;
	getrusage(RUSAGE_SELF, &ru);
	return (ru.ru_utime.tv_sec+ru.ru_stime.tv_sec)*sec + (ru.ru_utime.tv_usec+ru.ru_stime.tv_usec)*usec;
}

void sleep(const TimeDelta &d)
{
	timespec ts = rawtime_to_timespec(d.raw());
	while(nanosleep(&ts, 0)==-1)
		if(errno!=EINTR)
			throw system_error("nanosleep");
}

} // namespace Time
} // namespace Msp
