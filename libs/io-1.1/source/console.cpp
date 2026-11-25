/* $Id$

This file is part of libmspio
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef WIN32
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#endif
#include <msp/core/except.h>
#include "console.h"

namespace {

#ifndef WIN32
termios orig_attr;
#endif

} // namespace

namespace Msp {
namespace IO {

Console::Console(unsigned n)
{
	if(n>2)
		throw InvalidParameterValue("Invalid parameter for Console::Console");

	mode=(n==0 ? M_READ : M_WRITE);

#ifdef WIN32
	switch(n)
	{
	case 0: handle=GetStdHandle(STD_INPUT_HANDLE); break;
	case 1: handle=GetStdHandle(STD_OUTPUT_HANDLE); break;
	case 2: handle=GetStdHandle(STD_ERROR_HANDLE); break;
	}
#else
	handle=n;

	if(handle==0)
		tcgetattr(handle, &orig_attr);
#endif

	if(n==0)
		set_events(P_INPUT);
}

Console::~Console()
{
#ifndef WIN32
	if(handle==0)
		tcsetattr(handle, TCSADRAIN, &orig_attr);
#endif
}

void Console::set_block(bool b)
{
#ifdef WIN32
	// XXX Dunno how to do this in win32
	(void)b;
#else
	int flags=fcntl(0, F_GETFL);
	flags=(flags&~O_NONBLOCK) | (b?0:O_NONBLOCK);
	fcntl(0, F_SETFL, flags);
#endif
}

void Console::set_local_echo(bool e)
{
	if(!(mode&M_READ))
		throw InvalidState("Local echo can only be set on input console");

#ifdef WIN32
	DWORD m;
	GetConsoleMode(handle, &m);
	SetConsoleMode(handle, (m&~ENABLE_ECHO_INPUT) | (e?ENABLE_ECHO_INPUT:0));
#else
	termios t;
	tcgetattr(0, &t);
	t.c_lflag=(t.c_lflag&~ECHO) | (e?ECHO:0);
	tcsetattr(0, TCSADRAIN, &t);
#endif
}

void Console::set_line_buffer(bool l)
{
	if(!(mode&M_READ))
		throw InvalidState("Line buffering can only be set on input console");

#ifdef WIN32
	DWORD m;
	GetConsoleMode(handle, &m);
	SetConsoleMode(handle, (m&~ENABLE_LINE_INPUT) | (l?ENABLE_LINE_INPUT:0));
#else
	// XXX ICANON does more than just set line buffering, may need a bit more thought
	termios t;
	tcgetattr(0, &t);
	t.c_lflag=(t.c_lflag&~ICANON) | (l?ICANON:0);
	tcsetattr(0, TCSADRAIN, &t);
#endif
}

void Console::get_size(unsigned &rows, unsigned &cols)
{
	if(!(mode&M_WRITE))
		throw InvalidState("Size can only be queried from an output console");

#ifdef WIN32
	// XXX Figure out how to do this
	rows=24;
	cols=80;
#else
	struct winsize wsz;
	ioctl(handle, TIOCGWINSZ, &wsz);
	rows=wsz.ws_row;
	cols=wsz.ws_col;
#endif
}

Handle Console::get_event_handle()
{
	return 0;
}

unsigned Console::do_write(const char *buf, unsigned len)
{
	if(!(mode&M_WRITE))
		throw InvalidState("Console is not writable");

#ifdef WIN32
	DWORD ret;
	if(!WriteFile(handle, buf, len, &ret, 0))
		throw SystemError("Writing to console failed", GetLastError());
#else
	int ret=::write(handle, buf, len);
	if(ret==-1)
		throw SystemError("Writing to console failed", errno);
#endif

	return ret;
}

unsigned Console::do_read(char *buf, unsigned len)
{
	if(!(mode&M_READ))
		throw InvalidState("Console is not readable");

#ifdef WIN32
	DWORD ret;
	if(!ReadFile(handle, buf, len, &ret, 0))
		throw SystemError("Reading from console failed", GetLastError());
#else
	int ret=::read(handle, buf, len);
	if(ret==-1)
	{
		if(errno==EAGAIN)
			return 0;
		else
			throw SystemError("Reading from console failed", errno);
	}
	else if(ret==0)
		eof_flag=true;
#endif

	return ret;
}

Console &Console::instance(unsigned n)
{
	static Console in(0);
	static Console out(1);
	static Console err(2);

	switch(n)
	{
	case 0: return in;
	case 1: return out;
	case 2: return err;
	}

	throw InvalidParameterValue("Unknown Console instance requested");
}

Console &cin=Console::instance(0);
Console &cout=Console::instance(1);
Console &cerr=Console::instance(2);

} // namespace IO
} // namespace Msp
