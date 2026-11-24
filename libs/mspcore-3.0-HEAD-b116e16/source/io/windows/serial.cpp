#include <msp/core/systemerror.h>
#include <msp/strings/format.h>
#include "handle_private.h"
#include "serial.h"
#include "serial_private.h"

using namespace std;

namespace Msp {
namespace IO {

void Serial::platform_init(const string &port)
{
	string name = "\\\\.\\"+port;

	*handle = CreateFile(name.c_str(), GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, 0);
	if(!handle)
		throw system_error(format("CreateFile(%s)", port));
	mode = M_READ|M_WRITE;

	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = MAXDWORD;
	timeouts.ReadTotalTimeoutMultiplier = MAXDWORD;
	timeouts.ReadTotalTimeoutConstant = MAXDWORD-1;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(*handle, &timeouts);
}


void Serial::DeviceState::get_from(const Handle &h)
{
	GetCommState(*h, &state);
}

void Serial::DeviceState::apply_to(const Handle &h)
{
	if(SetCommState(*h, &state)==0)
		throw system_error("SetCommState");
}

void Serial::DeviceState::set_baud_rate(unsigned baud)
{
	state.BaudRate = baud;
}

void Serial::DeviceState::set_data_bits(unsigned bits)
{
	state.ByteSize = bits;
}

void Serial::DeviceState::set_parity(Serial::Parity par)
{
	switch(par)
	{
	case Serial::NONE: state.Parity = NOPARITY; break;
	case Serial::EVEN: state.Parity = EVENPARITY; break;
	case Serial::ODD:  state.Parity = ODDPARITY; break;
	default: throw invalid_argument("set_parity");
	}
}

void Serial::DeviceState::set_stop_bits(unsigned bits)
{
	switch(bits)
	{
	case 1: state.StopBits = ONESTOPBIT; break;
	case 2: state.StopBits = TWOSTOPBITS; break;
	default: throw invalid_argument("set_stop_bits");
	}
}

} // namespace IO
} // namespace MSp
