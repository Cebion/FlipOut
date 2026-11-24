#include <stdexcept>
#include "slice.h"

using namespace std;

namespace Msp {
namespace IO {

Slice::Slice(Seekable &b, SeekOffset s, SeekOffset l):
	below(b),
	start_offset(s),
	length(l),
	position(0),
	sync_position(true)
{
	if(s<0 || l<0)
		throw invalid_argument("Slice");

	Base::Synchronize sync(below);
	mode = below.get_mode()&M_RDWR;
	below.signal_flush_required.connect(sigc::mem_fun(this, &Slice::flush));
}

void Slice::set_block(bool)
{
	throw logic_error("Slice::set_block");
}

void Slice::set_inherit(bool)
{
	throw logic_error("Slice::set_inherit");
}

void Slice::flush()
{
	sync_position = true;
}

unsigned Slice::prepare_op(unsigned size, Mode m)
{
	check_access(m);

	if(sync_position)
	{
		SeekOffset pos = below.seek(start_offset+position, S_BEG)-start_offset;
		if(pos!=position)
			throw runtime_error("failed to restore position");
		sync_position = false;
	}

	SeekOffset remaining = length-position;
	if(size>remaining)
		size = remaining;
	if(!size && m==M_READ)
		set_eof();

	return size;
}

size_t Slice::do_write(const char *data, size_t size)
{
	Base::Synchronize sync(below);
	size = prepare_op(size, M_WRITE);
	if(!size)
		return 0;

	size_t len = below.write(data, size);
	position += len;
	return len;
}

size_t Slice::do_read(char *data, size_t size)
{
	Base::Synchronize sync(below);
	size = prepare_op(size, M_READ);
	if(!size)
		return 0;

	size_t len = below.read(data, size);
	if(!len && below.eof())
		set_eof();
	position += len;
	return len;
}

size_t Slice::put(char c)
{
	Base::Synchronize sync(below);
	if(!prepare_op(1, M_WRITE))
		return 0;

	size_t len = below.put(c);
	position += len;
	return len;
}

int Slice::get()
{
	Base::Synchronize sync(below);
	if(!prepare_op(1, M_READ))
		return -1;

	int c = below.get();
	if(c==-1 && below.eof())
		set_eof();
	else
		++position;
	return c;
}

const Handle &Slice::get_handle(Mode)
{
	throw logic_error("Slice::get_handle");
}

SeekOffset Slice::seek(SeekOffset off, SeekType type)
{
	Base::Synchronize sync(below);
	off += start_offset;
	if(type==S_CUR)
		off += position;
	else if(type==S_END)
		off += length;
	else if(type!=S_BEG)
		throw invalid_argument("Slice::seek");

	if(off<start_offset || off>start_offset+length)
		throw bad_seek(off, type);

	signal_flush_required.emit();

	position = below.seek(off, S_BEG)-start_offset;
	sync_position = false;
	return position;
}

} // namespace IO
} // namespace Msp
