#include "console.h"

using namespace std;

namespace Msp {
namespace IO {

Console::Console(Stream s):
	stream(s)
{
	mode = (stream==CIN ? M_READ : M_WRITE);

	platform_init();

	if(stream==CIN)
		set_events(P_INPUT);
}

void Console::set_block(bool b)
{
	adjust_mode(mode, M_NONBLOCK, !b);
	sys_set_blocking(handle, b);
}

void Console::set_inherit(bool i)
{
	adjust_mode(mode, M_INHERIT, i);
	sys_set_inherit(handle, i);
}

size_t Console::do_write(const char *buf, size_t len)
{
	check_access(M_WRITE);

	return sys_write(handle, buf, len);
}

size_t Console::do_read(char *buf, size_t len)
{
	check_access(M_READ);

	size_t ret = sys_read(handle, buf, len);
	if(ret==0)
		set_eof();

	return ret;
}

const Handle &Console::get_handle(Mode m)
{
	check_access(m);
	return handle;
}

Console &Console::instance(Stream s)
{
	static Console in(CIN);
	static Console out(COUT);
	static Console err(CERR);

	switch(s)
	{
	case CIN: return in;
	case COUT: return out;
	case CERR: return err;
	}

	throw invalid_argument("Console::instance");
}

Console &cin = Console::instance(Console::CIN);
Console &cout = Console::instance(Console::COUT);
Console &cerr = Console::instance(Console::CERR);

} // namespace IO
} // namespace Msp
