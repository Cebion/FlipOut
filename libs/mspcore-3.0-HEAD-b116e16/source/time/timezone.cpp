#include <cstdlib>
#include <msp/strings/format.h>
#include "timezone.h"

using namespace std;

namespace Msp {
namespace Time {

TimeZone::TimeZone():
	name("UTC")
{ }

TimeZone::TimeZone(int minutes):
	offset(minutes*min)
{
	if(minutes)
	{
		int m = abs(minutes);
		name = format("UTC%c%d", (minutes<0 ? '-' : '+'), m/60);
		if(m%60)
			name += format(":%02d", m%60);
	}
	else
		name = "UTC";
}

TimeZone::TimeZone(int minutes, const string &n):
	name(n),
	offset(minutes*min)
{ }

const TimeZone &TimeZone::utc()
{
	static TimeZone tz(0);
	return tz;
}

const TimeZone &TimeZone::local()
{
	static TimeZone tz = platform_get_local_timezone();
	return tz;
}

} // namespace Time
} // namespace Msp
