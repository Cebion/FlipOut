/* $Id: units.cpp 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore     
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "units.h"

namespace Msp {
namespace Time {

// Constants to be used in creation of TimeDeltas
const TimeDelta zero(0);
const TimeDelta usec(1);
const TimeDelta msec(1000);
const TimeDelta sec(1000000);
const TimeDelta min(60*1000000);
const TimeDelta hour(3600*1000000LL);
const TimeDelta day(86400*1000000LL);
const TimeDelta week(7*86400*1000000LL);

} // namespace Time
} // namespace Msp
