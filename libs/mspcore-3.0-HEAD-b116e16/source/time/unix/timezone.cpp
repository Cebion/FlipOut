#include <unistd.h>
#include <fcntl.h>
#include "timestamp.h"
#include "timezone.h"
#include "utils.h"

using namespace std;

namespace {

long get_long(char *&ptr)
{
	long result = 0;
	for(unsigned i=0; i<4; ++i)
		result = (result<<8)+static_cast<unsigned char >(*ptr++);
	return result;
}

}


namespace Msp {
namespace Time {

TimeZone TimeZone::platform_get_local_timezone()
{
	int fd = open("/etc/localtime", O_RDONLY);
	if(fd!=-1)
	{
		char hdr[44];
		int len = read(fd, hdr, sizeof(hdr));
		long gmtoff = -1;
		string name;
		if(len==44 && hdr[0]=='T' && hdr[1]=='Z' && hdr[2]=='i' && hdr[3]=='f')
		{
			char *ptr = hdr+20;
			long isgmtcnt = get_long(ptr);
			long isstdcnt = get_long(ptr);
			long leapcnt = get_long(ptr);
			long timecnt = get_long(ptr);
			long typecnt = get_long(ptr);
			long charcnt = get_long(ptr);
			int size = timecnt*5+typecnt*6+isgmtcnt+isstdcnt+leapcnt*8+charcnt;
			char *buf = new char[size];
			len = read(fd, buf, size);
			if(len==size)
			{
				ptr = buf;
				int index = -1;
				time_t cur_time = Msp::Time::now().to_unixtime();
				for(int i=0; i<timecnt; ++i)
					if(get_long(ptr)<=cur_time)
						index = i;

				if(index>0)
					index = ptr[index];
				ptr += timecnt;

				int abbrind = 0;
				for(int i=0; i<typecnt; ++i)
				{
					if((index>=0 && i==index) || (index<0 && !ptr[4] && gmtoff==-1))
					{
						gmtoff = get_long(ptr);
						++ptr;
						abbrind = *ptr++;
					}
					else
						ptr += 6;
				}

				name = ptr+abbrind;
			}
			delete[] buf;
		}
		close(fd);

		if(gmtoff!=-1)
			return TimeZone(gmtoff/60, name);
	}
	return TimeZone();
}

} // namespace Time
} // namespace Msp
