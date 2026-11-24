#ifndef MSP_IO_SERIAL_PRIVATE_H_
#define MSP_IO_SERIAL_PRIVATE_H_

#include "serial.h"
#include "serial_platform.h"

namespace Msp {
namespace IO {

struct Serial::DeviceState
{
	PlatformSerialDeviceState state;

	void get_from(const Handle &);
	void apply_to(const Handle &);
	void set_baud_rate(unsigned);
	void set_data_bits(unsigned);
	void set_parity(Serial::Parity);
	void set_stop_bits(unsigned);
};

} // namespace IO
} // namespace Msp

#endif
