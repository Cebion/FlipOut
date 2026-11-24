#include <fcntl.h>
#include <termios.h>
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
	string fn = port;
	if(fn.compare(0, 5, "/dev/"))
		fn = "/dev/"+fn;

	*handle = open(fn.c_str(), O_RDWR);
	if(!handle)
		throw system_error(format("open(%s)", port));
	mode = M_READ|M_WRITE;

	termios t;
	tcgetattr(*handle, &t);
	t.c_iflag &= ~(ISTRIP|INLCR|IGNCR|ICRNL|IXON);
	t.c_lflag &= ~(ECHO|ICANON|ISIG|IEXTEN);
	t.c_oflag &= ~(OPOST|OCRNL|ONOCR|ONLRET);
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 0;
	tcsetattr(*handle, TCSADRAIN, &t);
}


void Serial::DeviceState::get_from(const Handle &h)
{
	tcgetattr(*h, &state);
}

void Serial::DeviceState::apply_to(const Handle &h)
{
	if(tcsetattr(*h, TCSADRAIN, &state)==-1)
		throw system_error("tcsetattr");
}

void Serial::DeviceState::set_baud_rate(unsigned baud)
{
	speed_t speed;
	switch(baud)
	{
	case 0:      speed = B0; break;
	case 50:     speed = B50; break;
	case 75:     speed = B75; break;
	case 110:    speed = B110; break;
	case 134:    speed = B134; break;
	case 150:    speed = B150; break;
	case 200:    speed = B200; break;
	case 300:    speed = B300; break;
	case 600:    speed = B600; break;
	case 1200:   speed = B1200; break;
	case 1800:   speed = B1800; break;
	case 2400:   speed = B2400; break;
	case 4800:   speed = B4800; break;
	case 9600:   speed = B9600; break;
	case 19200:  speed = B19200; break;
	case 38400:  speed = B38400; break;
	case 57600:  speed = 0010001; break;
	case 115200: speed = 0010002; break;
	case 230400: speed = 0010003; break;
	case 460800: speed = 0010004; break;
	case 500000: speed = 0010005; break;
	case 576000: speed = 0010006; break;
	case 921600: speed = 0010007; break;
	case 1000000: speed = 0010010; break;
	case 1152000: speed = 0010011; break;
	case 1500000: speed = 0010012; break;
	case 2000000: speed = 0010013; break;
	case 2500000: speed = 0010014; break;
	case 3000000: speed = 0010015; break;
	case 3500000: speed = 0010016; break;
	case 4000000: speed = 0010017; break;
	default: throw invalid_argument("set_baud_rate");
	}

	int ret = cfsetospeed(&state, speed);
	if(ret==0)
		ret = cfsetispeed(&state, speed);
	if(ret<0)
		throw invalid_argument("set_baud_rate");
}

void Serial::DeviceState::set_data_bits(unsigned bits)
{
	tcflag_t flag;
	switch(bits)
	{
	case 5: flag = CS5; break;
	case 6: flag = CS6; break;
	case 7: flag = CS7; break;
	case 8: flag = CS8; break;
	default: throw invalid_argument("set_data_bits");
	}

	state.c_cflag = (state.c_cflag&~CSIZE)|flag;
}

void Serial::DeviceState::set_parity(Serial::Parity par)
{
	tcflag_t flag;
	switch(par)
	{
	case Serial::NONE: flag = 0; break;
	case Serial::EVEN: flag = PARENB; break;
	case Serial::ODD:  flag = PARENB|PARODD; break;
	default: throw invalid_argument("set_parity");
	}

	state.c_cflag = (state.c_cflag&~(PARENB|PARODD))|flag;
}

void Serial::DeviceState::set_stop_bits(unsigned bits)
{
	tcflag_t flag;
	switch(bits)
	{
	case 1: flag = 0; break;
	case 2: flag = CSTOPB; break;
	default: throw invalid_argument("set_stop_bits");
	}

	state.c_cflag = (state.c_cflag&~CSTOPB)|flag;
}

} // namespace IO
} // namespace MSp
