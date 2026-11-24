#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <msp/core/systemerror.h>
#include "console.h"
#include "handle_private.h"

namespace {

termios orig_attr;

}


namespace Msp {
namespace IO {

void Console::platform_init()
{
	*handle = stream;

	if(stream==CIN)
		tcgetattr(*handle, &orig_attr);
}

Console::~Console()
{
	if(stream==CIN)
		tcsetattr(*handle, TCSADRAIN, &orig_attr);
}

void Console::set_local_echo(bool e)
{
	check_access(M_READ);

	termios t;
	tcgetattr(*handle, &t);
	t.c_lflag = (t.c_lflag&~ECHO) | (e?ECHO:0);
	tcsetattr(*handle, TCSADRAIN, &t);
}

void Console::set_line_buffer(bool l)
{
	check_access(M_READ);

	termios t;
	if(tcgetattr(*handle, &t)==-1)
		throw system_error("tcgetattr");
	t.c_lflag = (t.c_lflag&~ICANON) | (l?ICANON:0);
	// man termios warns that VMIN and VTIME may share indices with VEOF and VEOL
	if(l)
	{
		t.c_cc[VEOF] = orig_attr.c_cc[VEOF];
		t.c_cc[VEOL] = orig_attr.c_cc[VEOL];
	}
	else
	{
		t.c_cc[VMIN] = 1;
		t.c_cc[VTIME] = 0;
	}
	if(tcsetattr(*handle, TCSADRAIN, &t)==-1)
		throw system_error("tcsetattr");
}

void Console::get_size(unsigned &rows, unsigned &cols)
{
	check_access(M_WRITE);

	struct winsize wsz;
	if(ioctl(*handle, TIOCGWINSZ, &wsz)==-1)
		throw system_error("ioctl TIOCGWINSZ");
	rows = wsz.ws_row;
	cols = wsz.ws_col;
}

void Console::redirect(Base &other)
{
	dup2(*other.get_handle(mode), *handle);
}

} // namespace IO
} // namespace Msp
