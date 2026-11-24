/* $Id: timedelta.cpp 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <sstream>
#include <iomanip>
#include "timedelta.h"
#include "units.h"

using namespace std;

namespace {

using Msp::Time::RawTime;

void print_part(ostream &out, RawTime &value, RawTime unit, char sep, bool &first)
{
	if(value<unit && first)
		return;
	
	if(!first)
		out<<sep<<setw(2);
	
	out<<value/unit;
	value%=unit;
	first=false;
}

}

namespace Msp {
namespace Time {

ostream &operator<<(ostream &out, const TimeDelta &td)
{
	ostringstream ss;
	ss.fill('0');

	RawTime value=td.raw();

	if(value<0)
	{
		ss<<'-';
		value=-value;
	}

	if(value==0)
		ss<<'0';
	else if(value<1000)
		ss<<value<<"µs";
	else if(value<1000000)
	{
		ss<<value/1000;
		value%=1000;
		if(value)
			ss<<'.'<<setw(3)<<value;
		ss<<"ms";
	}
	else
	{
		bool first=true;
		print_part(ss, value, 86400000000LL,  0,  first);
		print_part(ss, value, 3600000000LL,  '-', first);
		print_part(ss, value, 60000000LL,    ':', first);
		print_part(ss, value, 1000000LL,     ':', first);

		if(value)
		{
			ss<<'.';
			if(value%1000)
				ss<<setw(6)<<value;
			else
				ss<<setw(3)<<value/1000;
		}
		ss<<"s";
	}

	out<<ss.str();

	return out;
}

} // namespace Time
} // namespace Msp
