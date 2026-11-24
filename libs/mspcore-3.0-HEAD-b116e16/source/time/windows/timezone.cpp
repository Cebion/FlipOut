#include <windows.h>
#include <msp/core/systemerror.h>
#include "timezone.h"

namespace Msp {
namespace Time {

TimeZone TimeZone::platform_get_local_timezone()
{
	TIME_ZONE_INFORMATION tzinfo;
	DWORD dst = GetTimeZoneInformation(&tzinfo);
	if(dst==TIME_ZONE_ID_INVALID)
		throw Msp::system_error("GetTimeZoneInformation");

	int offset = tzinfo.Bias;
	if(dst==TIME_ZONE_ID_STANDARD)
		offset += tzinfo.StandardBias;
	else if(dst==TIME_ZONE_ID_DAYLIGHT)
		offset += tzinfo.DaylightBias;

	return TimeZone(offset);
}

} // namespace Time
} // namespace Msp
