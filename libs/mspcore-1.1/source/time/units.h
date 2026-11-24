/* $Id: units.h 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore     
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_TIME_UNITS_H_
#define MSP_TIME_UNITS_H_

#include "timedelta.h"

namespace Msp {
namespace Time {

extern const TimeDelta zero;
extern const TimeDelta usec;
extern const TimeDelta msec;
extern const TimeDelta sec;
extern const TimeDelta min;
extern const TimeDelta hour;
extern const TimeDelta day;
extern const TimeDelta week;

} // namespace Time
} // namespace Msp

#endif
