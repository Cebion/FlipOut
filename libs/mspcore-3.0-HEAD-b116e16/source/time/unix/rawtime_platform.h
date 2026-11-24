#ifndef MSP_TIME_RAWTIME_PLATFORM_H_
#define MSP_TIME_RAWTIME_PLATFORM_H_

#include <sys/time.h>
#include "rawtime.h"

namespace Msp {
namespace Time {

timeval rawtime_to_timeval(RawTime);
timespec rawtime_to_timespec(RawTime);
RawTime timeval_to_rawtime(const timeval &);

} // namespace Time
} // namespace Msp

#endif
