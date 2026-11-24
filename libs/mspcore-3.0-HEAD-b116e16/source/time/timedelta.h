#ifndef MSP_TIME_TIMEDELTA_H_
#define MSP_TIME_TIMEDELTA_H_

#include <cstdlib>
#include <ctime>
#include <msp/strings/lexicalcast.h>
#include "rawtime.h"

namespace Msp {
namespace Time {

/**
Represents a quantity of time, such as five seconds.
*/
class TimeDelta
{
private:
	RawTime usec;

public:
	/** Constructs a zero TimeDelta. */
	TimeDelta(): usec(0) { }

	/** Constructs a TimeDelta from a plain number.  The purpose of this is to
	allow serialization together with the raw() function. */
	explicit TimeDelta(RawTime u): usec(u) { }

	/** Returns the raw number stored inside the TimeDelta.  This should only be used
	for serialization and the result should not be interpreted in any way. */
	RawTime raw() const { return usec; }

	TimeDelta operator+(const TimeDelta &t) const { return TimeDelta(usec+t.usec); }
	TimeDelta &operator+=(const TimeDelta &t) { usec += t.usec; return *this; }
	TimeDelta operator-(const TimeDelta &t) const { return TimeDelta(usec-t.usec); }
	TimeDelta &operator-=(const TimeDelta &t) { usec -= t.usec; return *this; }
	TimeDelta operator-() const { return TimeDelta(-usec); }

	template<typename T>
	TimeDelta operator*(T a) const { return TimeDelta(RawTime(usec*a)); }
	template<typename T>
	TimeDelta &operator*=(T a) { usec = RawTime(usec*a); return *this; }

	template<typename T>
	TimeDelta operator/(T a) const { return TimeDelta(RawTime(usec/a)); }
	template<typename T>
	TimeDelta &operator/=(T a) { usec = RawTime(usec/a); return *this; }

	double operator/(const TimeDelta &t) const { return double(usec)/t.usec; }

	bool operator>(const TimeDelta &t) const { return usec>t.usec; }
	bool operator>=(const TimeDelta &t) const { return usec>=t.usec; }
	bool operator<(const TimeDelta &t) const { return usec<t.usec; }
	bool operator<=(const TimeDelta &t) const { return usec<=t.usec; }
	bool operator==(const TimeDelta &t) const { return usec==t.usec; }
	bool operator!=(const TimeDelta &t) const { return usec!=t.usec; }

	operator const void *() const { return usec ? this : 0; }
};

template<typename T>
inline TimeDelta operator*(T a, const TimeDelta &t) { return t*a; }

void operator<<(LexicalConverter &, const TimeDelta &);


// Constants to be used in creation of TimeDeltas
extern const TimeDelta zero;
extern const TimeDelta usec;
extern const TimeDelta msec;
extern const TimeDelta sec;
extern const TimeDelta min;
extern const TimeDelta hour;
extern const TimeDelta day;
extern const TimeDelta week;

inline TimeDelta abs(const TimeDelta &t) { return t>=zero ? t : -t; }
using std::abs;

} // namespace Time
} // namespace Msp

#endif
