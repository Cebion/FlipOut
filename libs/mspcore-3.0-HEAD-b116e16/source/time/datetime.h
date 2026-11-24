#ifndef MSP_TIME_DATETIME_H_
#define MSP_TIME_DATETIME_H_

#include <string>
#include "timezone.h"
#include "rawtime.h"

namespace Msp {
namespace Time {

class TimeDelta;
class TimeStamp;

/**
Provides handling of arbitary dates and times.  Can represent a moment of time
in the range of about ±2.1×10⁹ years.  It can also be formatted into a string
for presentation to the user.

Due to the complex internal representation, arithmetic operations on a DateTime
are relatively slow.  For purposes of internal scheduling in a program, a
TimeStamp is a better choice.
*/
class DateTime
{
private:
	int year;
	unsigned char month;
	unsigned char mday;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned usec;
	TimeZone zone;

public:
	DateTime(const TimeStamp &);
	DateTime(const TimeStamp &, const TimeZone &);
	DateTime(int, unsigned char, unsigned char);
	DateTime(int, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char);
	DateTime(int, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned);
private:
	void init(const TimeStamp &);
	void init(int, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned);

public:
	static DateTime parse_rfc3339(const std::string &);

	int get_year() const { return year; }
	unsigned char get_month() const { return month; }
	unsigned char get_mday() const { return mday; }
	unsigned char get_hour() const { return hour; }
	unsigned char get_minute() const { return minute; }
	unsigned char get_second() const { return second; }
	unsigned get_usec() const { return usec; }

	void add_days(int);
	void set_timezone(const TimeZone &);
	void convert_timezone(const TimeZone &);

	DateTime operator+(const TimeDelta &) const;
	DateTime &operator+=(const TimeDelta &);
	DateTime operator-(const TimeDelta &) const;
	DateTime &operator-=(const TimeDelta &);

	bool operator==(const DateTime &d) const { return cmp(d)==0; }
	bool operator!=(const DateTime &d) const { return cmp(d)!=0; }
	bool operator<(const DateTime &d) const { return cmp(d)<0; }
	bool operator<=(const DateTime &d) const { return cmp(d)<=0; }
	bool operator>(const DateTime &d) const { return cmp(d)>0; }
	bool operator>=(const DateTime &d) const { return cmp(d)>=0; }

	int cmp(const DateTime &) const;
	TimeStamp get_timestamp() const;
	std::string format(const std::string &) const;
	std::string format_rfc3339() const;

private:
	void add_raw(RawTime);
	void normalize();
};

inline void operator<<(LexicalConverter &c, const DateTime &d)
{ c.result(d.format_rfc3339()); }

inline void operator>>(const LexicalConverter &c, DateTime &d)
{ d = DateTime::parse_rfc3339(c.get()); }

} // namespace Time
} // namespace Msp

#endif
