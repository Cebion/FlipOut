#ifndef MSP_TIME_UTILS_H_
#define MSP_TIME_UTILS_H_

#include <string>

namespace Msp {
namespace Time {

class TimeDelta;
class TimeStamp;

/** Returns the current timestamp. */
TimeStamp now();

std::string format_now(const std::string &);

/** Returns the CPU time used by the program so far. */
TimeDelta get_cpu_time();

/** Sleeps for the given duration. */
void sleep(const TimeDelta &);

} // namespace Time
} // namespace Msp

#endif
