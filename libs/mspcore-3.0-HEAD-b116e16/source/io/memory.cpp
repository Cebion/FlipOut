#include <algorithm>
#include <cstring>
#include "handle.h"
#include "memory.h"

using namespace std;

namespace Msp {
namespace IO {

Memory::Memory(char *d, size_t s, Mode m)
{
	init(d, d+s, m);
}

Memory::Memory(char *b, char *e, Mode m)
{
	init(b, e, m);
}

Memory::Memory(const char *cd, size_t s)
{
	char *d = const_cast<char *>(cd);
	init(d, d+s, M_READ);
}

Memory::Memory(const char *b, const char *e)
{
	init(const_cast<char *>(b), const_cast<char *>(e), M_READ);
}

void Memory::init(char *b, char *e, Mode m)
{
	begin = b;
	end = e;
	pos = begin;
	mode = m;
}

void Memory::set_block(bool)
{
	throw logic_error("Memory::set_block");
}

void Memory::set_inherit(bool)
{
	throw logic_error("Memory::set_inherit");
}

size_t Memory::do_write(const char *buf, size_t size)
{
	check_access(M_WRITE);

	size = min<size_t>(size, end-pos);
	memcpy(pos, buf, size);
	pos += size;
	return size;
}

size_t Memory::do_read(char *buf, size_t size)
{
	check_access(M_READ);

	if(pos==end)
	{
		set_eof();
		return 0;
	}

	size = min<size_t>(size, end-pos);
	memcpy(buf, pos, size);
	pos += size;
	return size;
}

size_t Memory::put(char c)
{
	check_access(M_WRITE);

	if(pos==end)
		return 0;

	*pos++ = c;
	return 1;
}

bool Memory::getline(string &line)
{
	check_access(M_READ);

	if(pos==end)
	{
		set_eof();
		return false;
	}

	char *nl = find(pos, end, '\n');
	line.assign(pos, nl);
	pos = (nl==end ? end : nl+1);
	return true;
}

int Memory::get()
{
	check_access(M_READ);

	if(pos==end)
	{
		set_eof();
		return -1;
	}

	return static_cast<unsigned char>(*pos++);
}

const Handle &Memory::get_handle(Mode)
{
	throw logic_error("Memory::get_handle");
}

SeekOffset Memory::seek(SeekOffset off, SeekType type)
{
	char *new_pos;
	if(type==S_BEG)
		new_pos = begin+off;
	else if(type==S_CUR)
		new_pos = pos+off;
	else if(type==S_END)
		new_pos = end+off;
	else
		throw invalid_argument("Memory::seek");

	if(new_pos<begin || new_pos>end)
		throw bad_seek(off, type);

	pos = new_pos;
	eof_flag = false;
	return pos-begin;
}

} // namespace IO
} // namespace Msp
