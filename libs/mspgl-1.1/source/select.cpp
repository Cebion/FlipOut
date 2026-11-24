/* $Id: select.cpp 52 2008-07-19 11:19:01Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "except.h"
#include "gl.h"
#include "select.h"

using namespace std;

namespace {

vector<Msp::GL::SelectRecord> *select_buf=0;
vector<Msp::GL::uint> select_buf_int;

}

namespace Msp {
namespace GL {

void select_buffer(vector<SelectRecord> &buf)
{
	select_buf_int.resize(1024);
	glSelectBuffer(select_buf_int.size(), &select_buf_int[0]);
	select_buf=&buf;
}

void init_names()
{
	glInitNames();
}

void push_name(uint n)
{
	glPushName(n);
}

void pop_name()
{
	glPopName();
}

void load_name(uint n)
{
	glLoadName(n);
}

void parse_select_records(const uint *buf, uint count, vector<SelectRecord> &tbuf)
{
	uint i=0;
	while(count--)
	{
		SelectRecord record;

		uint n_names=buf[i++];
		record.min_depth=buf[i++];
		record.max_depth=buf[i++];

		record.names.reserve(n_names);
		while(n_names--)
			record.names.push_back(buf[i++]);
		
		tbuf.push_back(record);
	}
}

void _parse_internal_select_records(uint count)
{
	if(!select_buf)
		throw InvalidState("No select buffer specified");
	parse_select_records(&select_buf_int[0], count, *select_buf);
}

} // namespace GL
} // namespace Msp
