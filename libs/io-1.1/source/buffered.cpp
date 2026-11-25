/* $Id$

This file is part of libmspio
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cstring>
#include "buffered.h"
#include "except.h"

using namespace std;

namespace Msp {
namespace IO {

Buffered::Buffered(Base &b, unsigned s):
	below(b),
	buf_size(s),
	buf(new char[buf_size]),
	begin(buf),
	end(buf),
	cur_op(M_NONE)
{
	mode=below.get_mode();
	below.signal_flush_required.connect(sigc::mem_fun(this, &Buffered::flush));
}

unsigned Buffered::put(char c)
{
	set_op(M_WRITE);

	if(end<buf+buf_size)
	{
		*end++=c;
		return 1;
	}
	else
		return do_write(&c, 1);
}

void Buffered::flush()
{
	if(cur_op==M_WRITE)
	{
		unsigned used=end-begin;
		if(used)
		{
			unsigned len=below.write(begin, used);

			begin=end=buf;

			if(len<used)
				throw Exception("Couldn't flush all data");
		}
	}
	else if(cur_op==M_READ)
		begin=end=buf;
}

bool Buffered::getline(std::string &line)
{
	set_op(M_READ);

	for(char *i=begin; i!=end; ++i)
		if(*i=='\n')
		{
			line.assign(begin, i-begin);
			begin=i+1;
			return true;
		}

	return Base::getline(line);
}

int Buffered::get()
{
	set_op(M_READ);

	if(begin<end)
		return static_cast<unsigned char>(*begin++);

	char c;
	if(do_read(&c, 1)==0)
		return -1;
	return static_cast<unsigned char>(c);
}

Handle Buffered::get_event_handle()
{
	throw Exception("Buffered doesn't support events");
}

unsigned Buffered::get_current_size() const
{
	return end-begin;
}

Buffered::~Buffered()
{
	try
	{
		flush();
	}
	catch(...)
	{ }

	delete[] buf;
}

void Buffered::set_op(Mode op)
{
	if(op!=cur_op)
		flush();
	cur_op=op;
}

unsigned Buffered::do_write(const char *data, unsigned size)
{
	set_op(M_WRITE);

	if(end+size<buf+buf_size)
	{
		// All data fits in buffer with whatever is already there
		memcpy(end, data, size);
		end+=size;

		return size;
	}
	else
	{
		// Clear the buffer to make more room
		flush();

		if(size<buf_size)
		{
			// Put new data in the buffer to wait for more
			memcpy(end, data, size);
			end+=size;

			return size;
		}
		else
			// New data still doesn't fit in the buffer, so write it directly
			return below.write(data, size);
	}
}

unsigned Buffered::do_read(char *data, unsigned size)
{
	set_op(M_READ);

	if(begin+size<=end)
	{
		// The request can be served from the buffer
		memcpy(data, begin, size);
		begin+=size;

		eof_flag=(below.eof() && begin==end);

		return size;
	}
	else
	{
		// Give out whatever is in the buffer already
		memcpy(data, begin, end-begin);
		unsigned ret=end-begin;
		begin=end=buf;

		data+=ret;
		size-=ret;

		if(size<buf_size)
		{
			// Fill the buffer and serve the rest of the request from it
			unsigned len=below.read(end, buf+buf_size-end);
			end+=len;

			len=min(static_cast<unsigned>(end-begin), size);
			memcpy(data, begin, len);
			begin+=len;
			ret+=len;
		}
		else
			// Read the rest directly from the underlying object
			ret+=below.read(data, size);

		eof_flag=(below.eof() && begin==end);

		return ret;
	}
}

} // namespace IO
} // namespace Msp
