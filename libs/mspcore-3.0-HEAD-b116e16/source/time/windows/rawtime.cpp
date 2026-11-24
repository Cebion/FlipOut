#include "rawtime.h"
#include "rawtime_private.h"

namespace {

/// Returns the unixtime epoch as filetime
Msp::Time::RawTime get_epoch()
{
	SYSTEMTIME st;
	st.wYear = 1970;
	st.wMonth = 1;
	st.wDay = 1;
	st.wHour = 0;
	st.wMinute = 0;
	st.wSecond = 0;
	st.wMilliseconds = 0;

	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);
	return (ft.dwLowDateTime+(static_cast<Msp::Time::RawTime>(ft.dwHighDateTime)<<32))/10;
}

}

namespace Msp {
namespace Time {

RawTime filetime_to_rawtime(const FILETIME &ft)
{
	static RawTime epoch = get_epoch();
	
	return (ft.dwLowDateTime+(static_cast<RawTime>(ft.dwHighDateTime)<<32))/10-epoch;
}

} // namespace Time
} // namespace Msp
