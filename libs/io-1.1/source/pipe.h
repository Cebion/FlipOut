/* $Id$

This file is part of libmspio
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#ifndef MSP_IO_PIPE_H_
#define MSP_IO_PIPE_H_

#include "base.h"

namespace Msp {
namespace IO {

class Pipe: public Base
{
public:
	Pipe();
	void set_block(bool);
	void close();
	Handle get_event_handle();
	~Pipe();
private:
	Handle handle[2];
#ifdef WIN32
	OVERLAPPED *overlapped;
	Handle     event;
	unsigned   buf_size;
	char       *buffer;
	unsigned   buf_avail;
	char       *buf_next;
#endif

protected:
	unsigned do_write(const char *, unsigned);
	unsigned do_read(char *, unsigned);
};

} // namespace IO
} // namespace Msp

#endif
