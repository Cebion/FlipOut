/* $Id$

This file is part of libmspio
Copyright © 2009 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_IO_MEMORY_H_
#define MSP_IO_MEMORY_H_

#include "base.h"
#include "seek.h"

namespace Msp {
namespace IO {

class Memory: public Base
{
private:
	char *begin;
	char *end;
	char *pos;

public:
	Memory(char *, unsigned);
	Memory(char *, char *);
	Memory(const char *, unsigned);
	Memory(const char *, const char *);
private:
	void init(char *, char *, Mode);

public:
	virtual unsigned put(char);
	virtual bool getline(std::string &);
	virtual int get();
	unsigned seek(int, SeekType);
	unsigned tell() const { return pos-begin; }
	virtual Handle get_event_handle();
private:
	virtual unsigned do_write(const char *, unsigned);
	virtual unsigned do_read(char *, unsigned);
	void check_mode(Mode) const;
};

} // namespace IO
} // namespace Msp

#endif
