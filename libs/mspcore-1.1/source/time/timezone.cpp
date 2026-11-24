/* $Id: timezone.cpp 66 2009-09-11 17:32:58Z tdb $

This file is part of libmspcore
Copyright © 2008-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cstdlib>
#include <sstream>
#include <iomanip>
#ifdef WIN32
#include <windows.h>
#else
#include <fcntl.h>
#endif
#include <msp/core/except.h>
#include "timestamp.h"
#include "timezone.h"
#include "units.h"
#include "utils.h"

using namespace std;

namespace {

using Msp::Time::TimeZone;

#ifndef WIN32
long get_long(char *&ptr)
{
	long result=0;
	for(unsigned i=0; i<4; ++i)
		result=(result<<8)+static_cast<unsigned char >(*ptr++);
	return result;
}
#endif

TimeZone get_local_timezone()
{
#ifdef WIN32
	TIME_ZONE_INFORMATION tzinfo;
	DWORD dst=GetTimeZoneInformation(&tzinfo);
	if(dst==TIME_ZONE_ID_INVALID)
		throw Msp::SystemError("Failed to get time zone information", GetLastError());

	int offset=tzinfo.Bias;
	if(dst==TIME_ZONE_ID_STANDARD)
		offset+=tzinfo.StandardBias;
	else if(dst==TIME_ZONE_ID_DAYLIGHT)
		offset+=tzinfo.DaylightBias;

	return TimeZone(offset);
#else
	int fd=open("/etc/localtime", O_RDONLY);
	if(fd>=-1)
	{
		char hdr[44];
		int len=read(fd, hdr, sizeof(hdr));
		long gmtoff=-1;
		string name;
		if(len==44 && hdr[0]=='T' && hdr[1]=='Z' && hdr[2]=='i' && hdr[3]=='f')
		{
			char *ptr=hdr+20;
			long isgmtcnt=get_long(ptr);
			long isstdcnt=get_long(ptr);
			long leapcnt=get_long(ptr);
			long timecnt=get_long(ptr);
			long typecnt=get_long(ptr);
			long charcnt=get_long(ptr);
			int size=timecnt*5+typecnt*6+isgmtcnt+isstdcnt+leapcnt*8+charcnt;
			char buf[size];
			len=read(fd, buf, size);
			if(len==size)
			{
				ptr=buf;
				int index=-1;
				time_t cur_time=Msp::Time::now().to_unixtime();
				for(int i=0; i<timecnt; ++i)
					if(get_long(ptr)<=cur_time)
						index=i;

				if(index>0)
					index=ptr[index];
				ptr+=timecnt;

				int abbrind=0;
				for(int i=0; i<typecnt; ++i)
				{
					if((index>=0 && i==index) || (index<0 && !ptr[4] && gmtoff==-1))
					{
						gmtoff=get_long(ptr);
						++ptr;
						abbrind=*ptr++;
					}
					else
						ptr+=6;
				}

				name=ptr+abbrind;
			}
		}
		close(fd);

		if(gmtoff!=-1)
			return TimeZone(-gmtoff/60, name);
	}
	return TimeZone();
#endif
}

}

namespace Msp {
namespace Time {

TimeZone::TimeZone():
	name("UTC")
{ }

TimeZone::TimeZone(int minutes_west):
	offset(minutes_west*min)
{
	if(minutes_west)
	{
		ostringstream ss;
		ss.fill('0');
		int m=abs(minutes_west);
		ss<<"UTC"<<(minutes_west<0 ? '-' : '+')<<m/60;
		if(m%60)
			ss<<':'<<setw(2)<<m%60;
	}
	else
		name="UTC";
}

TimeZone::TimeZone(int minutes_west, const string &n):
	name(n),
	offset(minutes_west*min)
{ }

const TimeZone &TimeZone::utc()
{
	static TimeZone tz(0);
	return tz;
}

const TimeZone &TimeZone::local()
{
	static TimeZone tz=get_local_timezone();
	return tz;
}

} // namespace Time
} // namespace Msp
