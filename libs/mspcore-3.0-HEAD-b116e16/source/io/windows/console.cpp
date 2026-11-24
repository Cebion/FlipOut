#include <msp/core/systemerror.h>
#include "console.h"
#include "handle_private.h"

using namespace std;

namespace {

DWORD stream_to_sys(Msp::IO::Console::Stream stream)
{
	switch(stream)
	{
	case Msp::IO::Console::CIN: return STD_INPUT_HANDLE;
	case Msp::IO::Console::COUT: return STD_OUTPUT_HANDLE;
	case Msp::IO::Console::CERR: return STD_ERROR_HANDLE;
	default: throw invalid_argument("stream_to_sys");
	}
}

} // namespace


namespace Msp {
namespace IO {

void Console::platform_init()
{
	*handle = GetStdHandle(stream_to_sys(stream));
}

Console::~Console()
{ }

void Console::set_local_echo(bool e)
{
	check_access(M_READ);

	DWORD m;
	GetConsoleMode(*handle, &m);
	SetConsoleMode(*handle, (m&~ENABLE_ECHO_INPUT) | (e?ENABLE_ECHO_INPUT:0));
}

void Console::set_line_buffer(bool l)
{
	check_access(M_READ);

	DWORD m;
	if(!GetConsoleMode(*handle, &m))
		throw system_error("GetConsoleMode");
	if(!SetConsoleMode(*handle, (m&~ENABLE_LINE_INPUT) | (l?ENABLE_LINE_INPUT:0)))
		throw system_error("SetConsoleMode");
}

void Console::get_size(unsigned &rows, unsigned &cols)
{
	check_access(M_WRITE);

	CONSOLE_SCREEN_BUFFER_INFO sbi;
	if(!GetConsoleScreenBufferInfo(*handle, &sbi))
		throw system_error("GetConsoleScreenBufferInfo");
	// Right/bottom coords are inclusive
	rows = sbi.srWindow.Bottom+1-sbi.srWindow.Top;
	cols = sbi.srWindow.Right+1-sbi.srWindow.Left;
}

void Console::redirect(Base &other)
{
	SetStdHandle(stream_to_sys(stream), *other.get_handle(mode));
}

} // namespace IO
} // namespace Msp
