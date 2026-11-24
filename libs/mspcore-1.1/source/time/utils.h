/* $Id: utils.h 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore     
Copyright © 2006-2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_TIME_UTILS_H_
#define MSP_TIME_UTILS_H_

#include <string>

namespace Msp {
namespace Time {

class TimeDelta;
class TimeStamp;

extern TimeStamp now();
extern std::string format_now(const std::string &);
extern TimeDelta get_cpu_time();
extern int sleep(const TimeDelta &);

} // namespace Time
} // namespace Msp

#endif
