#ifndef MSP_TIME_RAWTIME_PLATFORM_H_
#define MSP_TIME_RAWTIME_PLATFORM_H_

#include <windows.h>
#include "rawtime.h"

namespace Msp {
namespace Time {

RawTime filetime_to_rawtime(const FILETIME &);

} // namespace Time
} // namespace Msp

#endif
