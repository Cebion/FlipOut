/* $Id: timezone.h 66 2009-09-11 17:32:58Z tdb $

This file is part of libmspcore
Copyright © 2008-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_TIME_TIMEZONE_H_
#define MSP_TIME_TIMEZONE_H_

#include "timedelta.h"

namespace Msp {
namespace Time {

class TimeZone
{
private:
	std::string name;
	TimeDelta offset;

public:
	TimeZone();
	TimeZone(int);
	TimeZone(int, const std::string &);
	
	const std::string &get_name() const { return name; }
	const TimeDelta &get_offset() const { return offset; }

	static const TimeZone &utc();
	static const TimeZone &local();
};

} // namespace Time
} // namespace Msp

#endif
