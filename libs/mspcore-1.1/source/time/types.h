/* $Id: types.h 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore     
Copyright © 2006,2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_TIME_TYPES_H_
#define MSP_TIME_TYPES_H_

#ifndef WIN32
#include <stdint.h>
#endif

namespace Msp {
namespace Time {

#ifdef WIN32
typedef __int64 RawTime;
#else
typedef int64_t RawTime;
#endif

} // namespace Time
} // namespace Msp

#endif
