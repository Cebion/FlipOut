#include <msp/strings/format.h>
#include "timedelta.h"

using namespace std;

namespace {

using Msp::Time::RawTime;

string format_part(RawTime &value, RawTime unit, char sep, bool &first)
{
	if(value<unit && first)
		return string();

	RawTime v = value/unit;
	value %= unit;
	if(first)
	{
		first = false;
		return Msp::format("%d", v);
	}
	else
		return Msp::format("%c%02d", sep, v);
}

}

namespace Msp {
namespace Time {

void operator<<(LexicalConverter &conv, const TimeDelta &td)
{
	string result;

	RawTime value = td.raw();

	if(value<0)
	{
		result += '-';
		value = -value;
	}

	if(value==0)
		result += '0';
	else if(value<1000)
		result += format("%dµs", value);
	else if(value<1000000)
	{
		result += format("%d", value/1000);
		value %= 1000;
		if(value)
			result += format(".%03d", value);
		result += "ms";
	}
	else
	{
		bool first = true;
		bool unit = value<60000000LL;
		result += format_part(value, 86400000000LL,  0,  first);
		result += format_part(value, 3600000000LL,  '-', first);
		result += format_part(value, 60000000LL,    ':', first);
		result += format_part(value, 1000000LL,     ':', first);

		if(value)
		{
			if(value%1000)
				result += format(".%06d", value);
			else
				result += format(".%03d", value/1000);
		}
		if(unit)
			result += 's';
	}

	conv.result(result);
}

const TimeDelta zero(0);
const TimeDelta usec(1);
const TimeDelta msec(1000);
const TimeDelta sec(1000000);
const TimeDelta min(60*1000000);
const TimeDelta hour(3600*1000000LL);
const TimeDelta day(86400*1000000LL);
const TimeDelta week(7*86400*1000000LL);

} // namespace Time
} // namespace Msp
