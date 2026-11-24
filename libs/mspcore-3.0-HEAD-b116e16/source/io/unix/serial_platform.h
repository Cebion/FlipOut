#ifndef MSP_IO_SERIAL_PLATFORM_H_
#define MSP_IO_SERIAL_PLATFORM_H_

#include <termios.h>

namespace Msp {
namespace IO {

typedef termios PlatformSerialDeviceState;

} // namespace IO
} // namespace Msp

#endif
