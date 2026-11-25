/* $Id$

This file is part of libmspio
Copyright © 2009 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <algorithm>
#include <cstring>
#include "except.h"
#include "memory.h"

using namespace std;

namespace Msp {
namespace IO {

Memory::Memory(char *d, unsigned s)
{
	init(d, d+s, M_RDWR);
}

Memory::Memory(char *b, char *e)
{
	init(b, e, M_RDWR);
}

Memory::Memory(const char *cd, unsigned s)
{
	char *d=const_cast<char *>(cd);
	init(d, d+s, M_READ);
}

Memory::Memory(const char *b, const char *e)
{
	init(const_cast<char *>(b), const_cast<char *>(e), M_READ);
}

void Memory::init(char *b, char *e, Mode m)
{
	begin=b;
	end=e;
	pos=begin;
	mode=m;
}

unsigned Memory::put(char c)
{
	check_mode(M_WRITE);
	*pos++=c;
	return 1;
}

bool Memory::getline(string &line)
{
	char *nl=find(pos, end, '\n');
	line.assign(pos, nl);
	bool result=(nl!=pos);
	pos=nl;
	return result;
}

int Memory::get()
{
	if(pos==end)
	{
		eof_flag=true;
		return -1;
	}

	return static_cast<unsigned char>(*pos++);
}

unsigned Memory::seek(int off, SeekType type)
{
	char *new_pos;
	if(type==S_BEG)
		new_pos=begin+off;
	else if(type==S_CUR)
		new_pos=pos+off;
	else if(type==S_END)
		new_pos=end+off;
	else
		throw InvalidParameterValue("Invalid seek type");

	if(new_pos<begin || new_pos>end)
		throw InvalidParameterValue("Invalid seek offset");

	pos=new_pos;
	return pos-begin;
}

Handle Memory::get_event_handle()
{
	throw Exception("Memory doesn't support events");
}

unsigned Memory::do_write(const char *buf, unsigned size)
{
	check_mode(M_WRITE);

	size=min<unsigned>(size, end-pos);
	memcpy(pos, buf, size);
	pos+=size;
	return size;
}

unsigned Memory::do_read(char *buf, unsigned size)
{
	if(pos==end)
	{
		eof_flag=true;
		return 0;
	}

	size=min<unsigned>(size, end-pos);
	memcpy(buf, pos, size);
	pos+=size;
	return size;
}

void Memory::check_mode(Mode m) const
{
	if(m==M_WRITE)
	{
		if(!(mode&M_WRITE))
			throw InvalidState("Memory is not writable");
		if(pos==end)
			throw InvalidState("Attempt to write past end of Memory");
	}
}

} // namespace IO
} // namespace Msp
