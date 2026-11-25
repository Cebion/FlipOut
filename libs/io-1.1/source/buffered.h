/* $Id$

This file is part of libmspio
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#ifndef MSP_IO_BUFFERED_H_
#define MSP_IO_BUFFERED_H_

#include "base.h"

namespace Msp {
namespace IO {

class Buffered: public Base
{
private:
	Base     &below;
	unsigned buf_size;
	char     *buf;
	char     *begin;
	char     *end;
	Mode     cur_op;

public:
	Buffered(Base &, unsigned =8192);
	~Buffered();

	unsigned put(char);
	void flush();
	bool getline(std::string &);
	int  get();
	Handle get_event_handle();
	Mode get_current_op() const { return cur_op; }
	unsigned get_current_size() const;
private:
	void set_op(Mode);
protected:
	unsigned do_write(const char *, unsigned);
	unsigned do_read(char *, unsigned);
};

} // namespace IO
} // namespace Msp

#endif
