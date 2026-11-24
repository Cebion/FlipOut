/* $Id: datetime.cpp 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore     
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cstdlib>
#include <sstream>
#include <iomanip>
#include "../core/except.h"
#include "datetime.h"
#include "timestamp.h"
#include "units.h"

using namespace std;

namespace {

inline bool is_leap_year(int y)
{ return y%4==0 && (y%100 || y%400==0); }

inline unsigned char month_days(int y, unsigned char m)
{
	switch(m)
	{
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	case 2:
		return is_leap_year(y)?29:28;
	default:
		return 31;
	}
}

template<typename T>
inline int cmp_(T a, T b)
{
	if(a<b)
		return -1;
	if(a>b)
		return 1;
	return 0;
}

}

namespace Msp {
namespace Time {

DateTime::DateTime(const TimeStamp &ts)
{
	init(ts);
}

DateTime::DateTime(const TimeStamp &ts, const TimeZone &tz)
{
	init(ts);
	convert_timezone(tz);
}

DateTime::DateTime(int y, unsigned char m, unsigned char d)
{
	init(y, m, d, 0, 0, 0, 0);
}

DateTime::DateTime(int y, unsigned char m, unsigned char d, unsigned char h, unsigned char n, unsigned char s)
{
	init(y, m, d, h, n, s, 0);
}

DateTime::DateTime(int y, unsigned char m, unsigned char d, unsigned char h, unsigned char n, unsigned char s, unsigned u)
{
	init(y, m, d, h, n, s, u);
}

void DateTime::add_days(int days)
{
	int new_year=year;

	/* Leap years have a 400 year cycle, so any 400 consecutive years have a
	constant number of days (400*365+97=146097) */
	new_year+=days/146097*400;
	days%=146097;

	if(days<0)
	{
		new_year-=400;
		days+=146097;
	}

	// Fudge factor for leap day
	int fudge=(month<=2)?1:0;

	// (Almost) every 4 year cycle has 1 leap year and 3 normal years
	unsigned cycles=days/1461;
	days%=1461;

	new_year+=cycles*4;

	// See how many non-leap-years we counted as leap years and reclaim the lost days
	// XXX This breaks with negative years
	unsigned missed_leap_days=((year-fudge)%100+cycles*4)/100;
	if((year-fudge)%400+cycles*4>=400)
		--missed_leap_days;

	days+=missed_leap_days;

	// Count single years from the 4 year cycle
	cycles=days/365;
	days%=365;

	new_year+=cycles;

	if((year-fudge)%4+cycles>=4)
	{
		// We passed a leap year - decrement days
		if(days==0)
		{
			days=is_leap_year(new_year-fudge)?365:364;
			--new_year;
		}
		else
			--days;
	}

	year=new_year;

	// Step months
	while(mday+days>month_days(year, month))
	{
		days-=month_days(year, month);
		++month;
		if(month>12)
		{
			++year;
			month=1;
		}
	}

	mday+=days;
}

void DateTime::set_timezone(const TimeZone &tz)
{
	zone=tz;
}

void DateTime::convert_timezone(const TimeZone &tz)
{
	add_raw((zone.get_offset()-tz.get_offset()).raw());
	zone=tz;
}

DateTime DateTime::operator+(const TimeDelta &td) const
{
	DateTime dt(*this);
	dt.add_raw(td.raw());
	return dt;
}

DateTime &DateTime::operator+=(const TimeDelta &td)
{
	add_raw(td.raw());
	return *this;
}

DateTime DateTime::operator-(const TimeDelta &td) const
{
	DateTime dt(*this);
	dt.add_raw(-td.raw());
	return dt;
}

DateTime &DateTime::operator-=(const TimeDelta &td)
{
	add_raw(-td.raw());
	return *this;
}

int DateTime::cmp(const DateTime &dt) const
{
	if(int c=cmp_(year, dt.year))
		return c;
	if(int c=cmp_(month, dt.month))
		return c;
	if(int c=cmp_(mday, dt.mday))
		return c;
	if(int c=cmp_(hour, dt.hour))
		return c;
	if(int c=cmp_(minute, dt.minute))
		return c;
	if(int c=cmp_(second, dt.second))
		return c;
	if(int c=cmp_(usec, dt.usec))
		return c;
	return 0;
}

TimeStamp DateTime::get_timestamp() const
{
	if(year<-289701 || year>293641)
		throw Exception("DateTime is not representable as a TimeStamp");

	RawTime raw=(((hour*60LL)+minute)*60+second)*1000000+usec;
	int days=(year-1970)*365;
	days+=(year-1)/4-(year-1)/100+(year-1)/400-477;
	for(unsigned i=1; i<month; ++i)
		days+=month_days(year, i);
	days+=mday-1;

	raw+=days*86400000000LL;

	return TimeStamp(raw);
}

string DateTime::format(const string &fmt) const
{
	ostringstream ss;
	ss.fill('0');
	for(string::const_iterator i=fmt.begin(); i!=fmt.end(); ++i)
	{
		if(*i=='%')
		{
			++i;
			if(i==fmt.end())
				break;
			else if(*i=='d')
				ss<<setw(2)<<int(mday);
			else if(*i=='H')
				ss<<setw(2)<<int(hour);
			else if(*i=='I')
				ss<<setw(2)<<hour%12;
			else if(*i=='m')
				ss<<setw(2)<<int(month);
			else if(*i=='M')
				ss<<setw(2)<<int(minute);
			else if(*i=='p')
				ss<<((hour>=12) ? "PM" : "AM");
			else if(*i=='S')
				ss<<setw(2)<<int(second);
			else if(*i=='y')
				ss<<setw(2)<<year%100;
			else if(*i=='Y')
				ss<<setw(4)<<internal<<year;
			else if(*i=='%')
				ss<<'%';
		}
		else
			ss<<*i;
	}

	return ss.str();
}

string DateTime::format_rfc3339() const
{
	string result=format("%Y-%m-%dT%H:%M:%S");
	if(const TimeDelta &offs=zone.get_offset())
	{
		ostringstream ss;
		ss.fill('0');
		int m=abs(static_cast<int>(offs/Time::min));
		ss<<(offs<zero ? '+' : '-')<<setw(2)<<m/60<<':'<<setw(2)<<m%60;
		result+=ss.str();
	}
	else
		result+='Z';
	return result;
}

void DateTime::init(const TimeStamp &ts)
{
	year=1970;
	month=1;
	mday=1;
	hour=0;
	minute=0;
	second=0;
	usec=0;
	add_raw(ts.raw());
}

void DateTime::init(int y, unsigned char m, unsigned char d, unsigned char h, unsigned char n, unsigned char s, unsigned u)
{
	year=y;
	month=m;
	mday=d;
	hour=h;
	minute=n;
	second=s;
	usec=u;
	validate();
}

void DateTime::add_raw(RawTime raw)
{
	int days=static_cast<int>(raw/86400000000LL);
	raw%=86400000000LL;
	if(raw<0)
	{
		--days;
		raw+=86400000000LL;
	}

	usec+=raw%1000000; raw/=1000000;
	second+=raw%60;    raw/=60;
	minute+=raw%60;    raw/=60;
	hour+=raw%24;      raw/=24;

	add_days(days);
	normalize();
}

void DateTime::normalize()
{
	second+=usec/1000000;
	usec%=1000000;
	minute+=second/60;
	second%=60;
	hour+=minute/60;
	minute%=60;
	mday+=hour/24;
	hour%=24;
	while(mday>month_days(year, month))
	{
		mday-=month_days(year, month);
		++month;
		if(month>12)
		{
			++year;
			month=1;
		}
	}
}

void DateTime::validate() const
{
	if(usec>=1000000)
		throw InvalidParameterValue("Microseconds out of range");
	if(second>=60)
		throw InvalidParameterValue("Seconds out of range");
	if(minute>=60)
		throw InvalidParameterValue("Minutes out of range");
	if(hour>=24)
		throw InvalidParameterValue("Hours out of range");
	if(month<1 || month>12)
		throw InvalidParameterValue("Month out of range");
	if(mday<1 || mday>month_days(year, month))
		throw InvalidParameterValue("Day of month out of range");
}

} // namespace Time
} // namespace Msp
