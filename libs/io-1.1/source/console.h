/* $Id$

This file is part of libmspio
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_IO_CONSOLE_H_
#define MSP_IO_CONSOLE_H_

#include "base.h"

namespace Msp {
namespace IO {

/**
Provides access to standard input, output and error streams.  This class can't
be instantiated directly - use one of the cin, cout and cerr references
instead.
*/
class Console: public Base
{
private:
	Handle handle;

	Console(unsigned);
public:
	~Console();

	virtual void set_block(bool);
	
	/** If local echo is enabled, characters will appear on the console as they
	are typed.  Can only be used on an input Console.  */
	void set_local_echo(bool);

	/** If line buffering is enabled, input will only be available when a
	newline is encountered.  On some systems, this may also enable line editing.
	Can only be used on an input Console.
	*/
	void set_line_buffer(bool);

	/**
	Retrieves the size of the Console.  Can only be used on an output Console.
	*/
	void get_size(unsigned &rows, unsigned &cols);

	virtual Handle get_event_handle();
protected:
	virtual unsigned do_write(const char *, unsigned);
	virtual unsigned do_read(char *, unsigned);
public:
	static Console &instance(unsigned);
};

extern Console &cin;
extern Console &cout;
extern Console &cerr;

} // namespace IO
} // namespace Msp

#endif
