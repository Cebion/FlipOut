/* $Id: timedelta.h 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore     
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_TIME_TIMEDELTA_H_
#define MSP_TIME_TIMEDELTA_H_

#include <time.h>
#include <ostream>
#include "types.h"

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
	/**
	Constructs a zero TimeDelta.
	*/
	TimeDelta(): usec(0) { }

	/**
	Constructs a TimeDelta from a plain number.  The purpose of this is to allow
	serialization together with the raw() function.  For creating TimeDeltas
	with a specific length, see units.h.
	*/
	explicit TimeDelta(RawTime u): usec(u) { }

	/**
	Returns the raw number stored inside the TimeDelta.  This should only be used
	for serialization and the result should not be interpreted in any way.
	*/
	RawTime raw() const { return usec; }

#ifndef WIN32
	/**
	Fills in a timespec struct.  To get a meaningful scalar value from the
	TimeDelta, divide with one of the values in units.h.
	*/
	void fill_timespec(timespec &ts) const { ts.tv_sec=usec/1000000; ts.tv_nsec=(usec%1000000)*1000; }
#endif

	TimeDelta operator+(const TimeDelta &t) const  { return TimeDelta(usec+t.usec); }
	TimeDelta &operator+=(const TimeDelta &t)      { usec+=t.usec; return *this; }
	TimeDelta operator-(const TimeDelta &t) const  { return TimeDelta(usec-t.usec); }
	TimeDelta &operator-=(const TimeDelta &t)      { usec-=t.usec; return *this; }

	template<typename T>
	TimeDelta operator*(T a) const                 { return TimeDelta(RawTime(usec*a)); }
	template<typename T>
	TimeDelta &operator*=(T a)                     { usec=RawTime(usec*a); return *this; }

	template<typename T>
	TimeDelta operator/(T a) const                 { return TimeDelta(RawTime(usec/a)); }
	template<typename T>
	TimeDelta &operator/=(T a)                     { usec=RawTime(usec/a); return *this; }

	double    operator/(const TimeDelta &t) const  { return double(usec)/t.usec; }

	bool      operator>(const TimeDelta &t) const  { return usec>t.usec; }
	bool      operator>=(const TimeDelta &t) const { return usec>=t.usec; }
	bool      operator<(const TimeDelta &t) const  { return usec<t.usec; }
	bool      operator<=(const TimeDelta &t) const { return usec<=t.usec; }
	bool      operator==(const TimeDelta &t) const { return usec==t.usec; }
	bool      operator!=(const TimeDelta &t) const { return usec!=t.usec; }

	operator const void *() const                  { return usec ? this : 0; }
};

template<typename T>
inline TimeDelta operator*(T a, const TimeDelta &t)   { return t*a; }

extern std::ostream &operator<<(std::ostream &, const TimeDelta &);

} // namespace Time
} // namespace Msp

#endif
