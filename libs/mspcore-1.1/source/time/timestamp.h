/* $Id: timestamp.h 66 2009-09-11 17:32:58Z tdb $

This file is part of libmspcore     
Copyright © 2006, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_TIME_TIMESTAMP_H_
#define MSP_TIME_TIMESTAMP_H_

#include "timedelta.h"
#include "types.h"

namespace Msp {
namespace Time {

/**
Represents a moment in time.  The main source of TimeStamps is the now()
function.

For representing user-specified times, use the DateTime class.
*/
class TimeStamp
{
private:
	RawTime usec;

public:
	/**
	Construct a TimeStamp that represents an arbitarily distant point in the
	past.  It's guaranteed to be less than any valid timestamp.
	*/
	TimeStamp(): usec(0) { }

	/**
	Constructs a TimeStamp from a plain number.  The purpose of this is to allow
	serialization together with the raw() function.
	*/
	explicit TimeStamp(RawTime u): usec(u) { }

	/**
	Returns the raw number stored inside the TimeStamp.  This value should be
	considered opaque and only be used for serialization.
	*/
	RawTime raw() const { return usec; }

	time_t to_unixtime() { return usec/1000000LL; }

	TimeStamp operator+(const TimeDelta &t) const  { return TimeStamp(usec+t.raw()); }
	TimeStamp &operator+=(const TimeDelta &t)      { usec+=t.raw(); return *this; }
	TimeStamp operator-(const TimeDelta &t) const  { return TimeStamp(usec-t.raw()); }
	TimeStamp &operator-=(const TimeDelta &t)      { usec-=t.raw(); return *this; }
	TimeDelta operator-(const TimeStamp &t) const  { return TimeDelta(usec-t.usec); }

	bool      operator>=(const TimeStamp &t) const { return usec>=t.usec; }
	bool      operator>(const TimeStamp &t) const  { return usec>t.usec; }
	bool      operator<=(const TimeStamp &t) const { return usec<=t.usec; }
	bool      operator<(const TimeStamp &t) const  { return usec<t.usec; }
	bool      operator==(const TimeStamp &t) const { return usec==t.usec; }
	bool      operator!=(const TimeStamp &t) const { return usec!=t.usec; }

	operator const void *() const                  { return usec>0 ? this : 0; }

	static TimeStamp from_unixtime(time_t t) { return TimeStamp(t*1000000LL); }
};

} // namespace Time
} // namespace Msp

#endif
