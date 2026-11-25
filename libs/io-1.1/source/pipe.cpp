/* $Id$

This file is part of libmspio
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef WIN32
#include <fcntl.h>
#include <errno.h>
#endif
#include <msp/strings/formatter.h>
#include "pipe.h"

using namespace std;

namespace Msp {
namespace IO {

Pipe::Pipe()
{
#ifdef WIN32
	string name=format("\\\\.\\pipe\\%u.%p", GetCurrentProcessId(), this);
	handle[0]=CreateNamedPipe(name.c_str(), PIPE_ACCESS_INBOUND|FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE, 1, 1024, 1024, 0, 0);
	if(handle[0]==INVALID_HANDLE_VALUE)
		throw SystemError("Unable to create pipe", GetLastError());

	handle[1]=CreateFile(name.c_str(), GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	if(handle[1]==INVALID_HANDLE_VALUE)
	{
		unsigned err=GetLastError();
		CloseHandle(handle[0]);
		throw SystemError("Unable to create pipe", err);
	}

	overlapped=0;
	event=CreateEvent(0, true, false, 0);
	buf_size=1024;
	buffer=new char[buf_size];
	buf_avail=0;
	buf_next=buffer;
#else
	if(pipe(handle)==-1)
		throw SystemError("Unable to create pipe", errno);
#endif

	set_events(P_INPUT);
}

void Pipe::set_block(bool b)
{
	mode=(mode&~M_NONBLOCK);
	if(b)
		mode=(mode|M_NONBLOCK);

#ifndef WIN32
	int flags=fcntl(handle[0], F_GETFD);
	fcntl(handle[0], F_SETFL, (flags&O_NONBLOCK)|(b?0:O_NONBLOCK));
	flags=fcntl(handle[1], F_GETFD);
	fcntl(handle[1], F_SETFL, (flags&O_NONBLOCK)|(b?0:O_NONBLOCK));
#endif
}

void Pipe::close()
{
	set_events(P_NONE);

	signal_flush_required.emit();
#ifdef WIN32
	CloseHandle(handle[0]);
	CloseHandle(handle[1]);
#else
	::close(handle[0]);
	::close(handle[1]);
	signal_closed.emit();
#endif
}

Handle Pipe::get_event_handle()
{
#ifdef WIN32
	if(!overlapped && !buf_avail)
	{
		overlapped=new OVERLAPPED;
		memset(overlapped, 0, sizeof(OVERLAPPED));
		overlapped->hEvent=event;

		DWORD ret;
		buf_next=buffer;
		if(!ReadFile(handle[0], buffer, buf_size, &ret, overlapped))
		{
			unsigned err=GetLastError();
			if(err!=ERROR_IO_PENDING)
				throw SystemError("Failed to start an overlapped read", err);
		}
		else
		{
			buf_avail=ret;
			delete overlapped;
			overlapped=0;
			SetEvent(event);
		}
	}

	return event;
#else
	return handle[0];
#endif
}

Pipe::~Pipe()
{
	close();
}

unsigned Pipe::do_write(const char *buf, unsigned size)
{
	if(size==0)
		return 0;

#ifdef WIN32
	DWORD ret;
	if(!WriteFile(handle[1], buf, size, &ret, 0))
		throw SystemError("Writing to pipe failed", GetLastError());
#else
	int ret=::write(handle[1], buf, size);
	if(ret==-1)
	{
		if(errno==EAGAIN)
			return 0;
		else
			throw SystemError("Writing to pipe failed", errno);
	}
#endif

	return ret;
}

unsigned Pipe::do_read(char *buf, unsigned size)
{
	if(size==0)
		return 0;

#ifdef WIN32
	// Initiate overlapped read if needed
	get_event_handle();

	if(overlapped)
	{
		DWORD ret;
		if(!GetOverlappedResult(handle[0], overlapped, &ret, !buf_avail))
			throw SystemError("Reading from pipe failed", GetLastError());
		else
		{
			buf_avail+=ret;
			delete overlapped;
			overlapped=0;
		}
	}

	unsigned ret=min(buf_avail, size);
	memcpy(buf, buf_next, ret);
	buf_next+=ret;
	buf_avail-=ret;

	// Initiate another overlapped read in case someone is polling us
	get_event_handle();
#else
	int ret=::read(handle[0], buf, size);
	if(ret==-1)
	{
		if(errno==EAGAIN)
			return 0;
		else
			throw SystemError("Reading from pipe failed", errno);
	}
#endif

	if(ret==0)
	{
		eof_flag=true;
		signal_end_of_file.emit();
	}

	return ret;
}

} // namespace IO
} // namespace Msp
