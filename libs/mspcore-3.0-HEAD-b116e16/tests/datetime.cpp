#include <msp/time/datetime.h>
#include <msp/time/timedelta.h>
#include <msp/time/timestamp.h>
#include <msp/time/timezone.h>
#include <msp/test/test.h>

using namespace std;
using namespace Msp;

class DateTimeTests: public Test::RegisteredTest<DateTimeTests>
{
public:
	DateTimeTests();

	static const char *get_name() { return "DateTime"; }

private:
	void add_days();
	void add_timedelta();
	void leap_year();
	void format();
	void timestamp();
	void timezone();
};


DateTimeTests::DateTimeTests()
{
	add(&DateTimeTests::add_days, "add_days");
	add(&DateTimeTests::add_timedelta, "+= TimeDelta");
	add(&DateTimeTests::leap_year, "Leap years");
	add(&DateTimeTests::format, "Formatting");
	add(&DateTimeTests::timestamp, "Timestamp conversion");
	add(&DateTimeTests::timezone, "Timezone conversion");
}

void DateTimeTests::add_days()
{
	Time::DateTime dt(2010, 1, 1);
	dt.add_days(1);
	EXPECT_EQUAL(dt, Time::DateTime(2010, 1, 2));
	dt.add_days(57);
	EXPECT_EQUAL(dt, Time::DateTime(2010, 2, 28));
	dt.add_days(307);
	EXPECT_EQUAL(dt, Time::DateTime(2011, 1, 1));
	dt.add_days(-366);
	EXPECT_EQUAL(dt, Time::DateTime(2009, 12, 31));
}

void DateTimeTests::add_timedelta()
{
	Time::DateTime dt(2010, 1, 1, 23, 50, 0);
	dt += 20*Time::min;
	EXPECT_EQUAL(dt, Time::DateTime(2010, 1, 2, 0, 10, 0));
	dt += 23*Time::hour+50*Time::min;
	EXPECT_EQUAL(dt, Time::DateTime(2010, 1, 3));
	dt += 52*Time::week;
	EXPECT_EQUAL(dt, Time::DateTime(2011, 1, 2));
}

void DateTimeTests::leap_year()
{
	Time::DateTime dt(1996, 2, 1);
	dt.add_days(29);
	EXPECT_EQUAL(dt, Time::DateTime(1996, 3, 1));

	dt = Time::DateTime(2000, 2, 1);
	dt.add_days(29);
	EXPECT_EQUAL(dt, Time::DateTime(2000, 3, 1));

	dt = Time::DateTime(2100, 2, 1);
	dt.add_days(29);
	EXPECT_EQUAL(dt, Time::DateTime(2100, 3, 2));

	dt = Time::DateTime(1996, 2, 1);
	dt.add_days(8*365);
	EXPECT_EQUAL(dt, Time::DateTime(2004, 1, 30));

	dt = Time::DateTime(2096, 2, 1);
	dt.add_days(8*365);
	EXPECT_EQUAL(dt, Time::DateTime(2104, 1, 31));

	dt = Time::DateTime(2010, 2, 1);
	dt.add_days(400*365+96);
	EXPECT_EQUAL(dt, Time::DateTime(2410, 1, 31));
}

void DateTimeTests::format()
{
	Time::DateTime dt(2010, 4, 27, 13, 57, 12);
	EXPECT_EQUAL(dt.format("%Y-%m-%d"), "2010-04-27");
	EXPECT_EQUAL(dt.format("%d%m%y"), "270410");
	EXPECT_EQUAL(dt.format("%H:%M:%S"), "13:57:12");
	EXPECT_EQUAL(dt.format("%I:%M %p"), "01:57 PM");
	EXPECT_EQUAL(Time::DateTime(2010, 4, 27, 12, 0, 0).format("%p"), "PM");
	EXPECT_EQUAL(dt.format_rfc3339(), "2010-04-27T13:57:12Z");
	dt.set_timezone(Time::TimeZone(120));
	EXPECT_EQUAL(dt.format_rfc3339(), "2010-04-27T13:57:12+02:00");
}

void DateTimeTests::timestamp()
{
	Time::DateTime dt(2010, 1, 1);
	Time::TimeStamp ts = dt.get_timestamp();
	Time::DateTime dt2(ts);
	EXPECT_EQUAL(dt, dt2);
}

void DateTimeTests::timezone()
{
	Time::DateTime dt(2010, 1, 1, 2, 0, 0);
	dt.convert_timezone(Time::TimeZone(120));
	EXPECT_EQUAL(dt, Time::DateTime(2010, 1, 1, 4, 0, 0));
	dt.convert_timezone(Time::TimeZone(-300));
	EXPECT_EQUAL(dt, Time::DateTime(2009, 12, 31, 21, 0, 0));
	dt.convert_timezone(Time::TimeZone::utc());
	EXPECT_EQUAL(dt, Time::DateTime(2010, 1, 1, 2, 0, 0));
}
