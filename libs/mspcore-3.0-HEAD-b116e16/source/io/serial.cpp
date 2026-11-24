#include "serial.h"
#include "serial_private.h"

using namespace std;

namespace Msp {
namespace IO {

Serial::Serial(const string &descr):
	reader(handle, 1024)
{
	string::size_type comma = descr.find(',');
	string port = descr.substr(0, comma);

	platform_init(port);

	if(comma!=string::npos)
	{
		try
		{
			set_parameters(descr.substr(comma+1));
		}
		catch(...)
		{
			close();
			throw;
		}
	}

	set_events(P_INPUT);
}

Serial::~Serial()
{
	close();
}

void Serial::close()
{
	sys_close(handle);
}

void Serial::set_block(bool b)
{
	adjust_mode(mode, M_NONBLOCK, !b);
	sys_set_blocking(handle, b);
}

void Serial::set_inherit(bool i)
{
	adjust_mode(mode, M_INHERIT, i);
	sys_set_inherit(handle, i);
}

void Serial::set_baud_rate(unsigned rate)
{
	DeviceState state;
	state.get_from(handle);
	state.set_baud_rate(rate);
	state.apply_to(handle);
}

void Serial::set_data_bits(unsigned bits)
{
	DeviceState state;
	state.get_from(handle);
	state.set_data_bits(bits);
	state.apply_to(handle);
}

void Serial::set_parity(Parity par)
{
	DeviceState state;
	state.get_from(handle);
	state.set_parity(par);
	state.apply_to(handle);
}

void Serial::set_stop_bits(unsigned bits)
{
	DeviceState state;
	state.get_from(handle);
	state.set_stop_bits(bits);
	state.apply_to(handle);
}

void Serial::set_parameters(const string &params)
{
	unsigned i;
	for(i=0; i<params.size() && isdigit(params[i]); ++i) ;
	if(i+4!=params.size() || params[i]!=',')
		throw invalid_argument("Serial::set_parameters");
	if(params[i+1]<'5' || params[i+1]>'8')
		throw invalid_argument("Serial::set_parameters data_bits");
	if(params[i+2]!='N' && params[i+2]!='E' && params[i+2]!='O')
		throw invalid_argument("Serial::set_parameters parity");
	if(params[i+3]!='1' && params[i+3]!='2')
		throw invalid_argument("Serial::set_parameters stop_bits");

	DeviceState state;
	state.get_from(handle);
	state.set_baud_rate(lexical_cast<unsigned>(params.substr(0, i)));
	state.set_data_bits(params[i+1]-'0');
	state.set_parity((params[i+2]=='E' ? EVEN : params[i+2]=='O' ? ODD : NONE));
	state.set_stop_bits(params[i+3]-'0');
	state.apply_to(handle);
}

unsigned Serial::do_write(const char *buf, unsigned size)
{
	if(size==0)
		return 0;

	return sys_write(handle, buf, size);
}

unsigned Serial::do_read(char *buf, unsigned size)
{
	if(size==0)
		return 0;

	unsigned ret = reader.read(buf, size);
	if(ret==0)
		set_eof();

	return ret;
}

const Handle &Serial::get_handle(Mode m)
{
	check_access(m);
	return handle;
}

} // namespace IO
} // namespace Msp
