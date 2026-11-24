#include <msp/time/timedelta.h>
#include <msp/time/timezone.h>
#include <msp/test/test.h>

using namespace std;
using namespace Msp;

class TimeZoneTests: public Test::RegisteredTest<TimeZoneTests>
{
public:
	TimeZoneTests();

	static const char *get_name() { return "TimeZone"; }

private:
	void utc();
	void local();
	void custom();
};


TimeZoneTests::TimeZoneTests()
{
	add(&TimeZoneTests::utc, "UTC");
	add(&TimeZoneTests::local, "Local");
	add(&TimeZoneTests::custom, "Custom");
}

void TimeZoneTests::utc()
{
	Time::TimeZone tz = Time::TimeZone::utc();
	EXPECT_EQUAL(tz.get_name(), "UTC");
	EXPECT_EQUAL(tz.get_offset(), Time::zero);
}

void TimeZoneTests::local()
{
	Time::TimeZone tz = Time::TimeZone::local();
	info(format("'%s' %s", tz.get_name(), tz.get_offset()));
}

void TimeZoneTests::custom()
{
	Time::TimeZone tz1(120);
	EXPECT_EQUAL(tz1.get_name(), "UTC+2");
	Time::TimeZone tz2(345);
	EXPECT_EQUAL(tz2.get_name(), "UTC+5:45");
}
