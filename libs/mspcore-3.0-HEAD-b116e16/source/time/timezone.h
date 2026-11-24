#ifndef MSP_TIME_TIMEZONE_H_
#define MSP_TIME_TIMEZONE_H_

#include "timedelta.h"

namespace Msp {
namespace Time {

class TimeZone
{
private:
	std::string name;
	TimeDelta offset;

public:
	TimeZone();
	TimeZone(int);
	TimeZone(int, const std::string &);
	
	const std::string &get_name() const { return name; }
	const TimeDelta &get_offset() const { return offset; }

	static const TimeZone &utc();
	static const TimeZone &local();
private:
	static TimeZone platform_get_local_timezone();
};

} // namespace Time
} // namespace Msp

#endif
