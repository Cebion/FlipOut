#include "rawtime.h"
#include "rawtime_private.h"

namespace Msp {
namespace Time {

timeval rawtime_to_timeval(RawTime raw)
{
	timeval tv;
	tv.tv_sec = raw/1000000;
	tv.tv_usec = raw%1000000;
	return tv;
}

timespec rawtime_to_timespec(RawTime raw)
{
	timespec ts;
	ts.tv_sec = raw/1000000;
	ts.tv_nsec = (raw%1000000)*1000;
	return ts;
}

RawTime timeval_to_rawtime(const timeval &tv)
{
	return tv.tv_sec*1000000LL+tv.tv_usec;
}

} // namespace Time
} // namespace Msp
