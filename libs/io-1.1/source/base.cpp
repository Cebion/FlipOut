/* $Id$

This file is part of libmspio
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#include "base.h"
#include "poll.h"

using namespace std;

namespace Msp {
namespace IO {

bool Base::getline(string &line)
{
	line.clear();

	if(eof_flag)
		return false;

	while(1)
	{
		int c=get();
		if(c==-1 || c=='\n')
			break;
		line+=c;
	}

	return !eof_flag || !line.empty();
}

int Base::get()
{
	char c;
	if(do_read(&c, 1)==0)
		return -1;
	return static_cast<unsigned char>(c);
}

void Base::event(PollEvent ev)
{
	if(ev&P_INPUT)
		signal_data_available.emit();

	on_event(ev);
}

Base::~Base()
{
	signal_deleted.emit();
}

Base::Base():
	mode(M_READ),
	events(P_NONE),
	eof_flag(false)
{ }

void Base::set_events(PollEvent e)
{
	events=e;
	signal_events_changed.emit(events);
}

} // namespace IO
} // namespace Msp
