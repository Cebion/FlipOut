#include "pipe.h"

using namespace std;

namespace Msp {
namespace IO {

Pipe::Pipe():
	reader(read_handle, 1024)
{
	mode = M_RDWR;

	platform_init();

	set_events(P_INPUT);
}

Pipe::~Pipe()
{
	set_events(P_NONE);

	signal_flush_required.emit();
	sys_close(read_handle);
	sys_close(write_handle);
}

void Pipe::set_mode(Mode m)
{
	m = m&M_RDWR;
	if(!m)
		throw invalid_argument("Pipe::set_mode");

	check_access(m);

	Mode close = mode&M_RDWR&~m;
	if(close&M_READ)
		sys_close(read_handle);
	if(close&M_WRITE)
		sys_close(write_handle);
	mode = (mode&~M_RDWR)|m;
}

void Pipe::set_block(bool b)
{
	adjust_mode(mode, M_NONBLOCK, !b);
	sys_set_blocking(read_handle, b);
	sys_set_blocking(write_handle, b);
}

void Pipe::set_inherit(bool i)
{
	adjust_mode(mode, M_INHERIT, i);
	sys_set_inherit(read_handle, i);
	sys_set_inherit(write_handle, i);
}

size_t Pipe::do_write(const char *buf, size_t size)
{
	if(size==0)
		return 0;

	return sys_write(write_handle, buf, size);
}

size_t Pipe::do_read(char *buf, size_t size)
{
	if(size==0)
		return 0;

	size_t ret = reader.read(buf, size);
	if(ret==0)
		set_eof();

	return ret;
}

const Handle &Pipe::get_handle(Mode m)
{
	if(m==M_READ)
		return read_handle;
	else if(m==M_WRITE)
		return write_handle;
	else
		throw invalid_argument("Pipe::get_handle");
}

} // namespace IO
} // namespace Msp
