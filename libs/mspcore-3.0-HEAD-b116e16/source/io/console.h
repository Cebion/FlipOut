#ifndef MSP_IO_CONSOLE_H_
#define MSP_IO_CONSOLE_H_

#include "eventobject.h"
#include "handle.h"

namespace Msp {
namespace IO {

/**
Provides access to standard input, output and error streams.  This class can't
be instantiated directly - use one of the cin, cout and cerr references
instead.
*/
class Console: public EventObject
{
public:
	enum Stream
	{
		CIN = 0,
		COUT = 1,
		CERR = 2
	};

private:
	Stream stream;
	Handle handle;

	Console(Stream);
	void platform_init();
public:
	~Console();

	virtual void set_block(bool);
	virtual void set_inherit(bool);
	
	/** If local echo is enabled, characters will appear on the console as they
	are typed.  Can only be used on an input Console.  */
	void set_local_echo(bool);

	/** If line buffering is enabled, input will only be available when a
	newline is encountered.  On some systems, this may also enable line editing.
	Can only be used on an input Console.
	*/
	void set_line_buffer(bool);

	/** Retrieves the size of the Console.  Can only be used on an output
	Console. */
	void get_size(unsigned &rows, unsigned &cols);

	/** Redirects input from or output to another I/O object.  This performs a
	system-level redirect.  Closing the other object won't affect the
	redirection. */
	void redirect(Base &);

protected:
	virtual std::size_t do_write(const char *, std::size_t);
	virtual std::size_t do_read(char *, std::size_t);

public:
	virtual const Handle &get_handle(Mode);
	virtual const Handle &get_event_handle() { return handle; }

	static Console &instance(Stream);
};

extern Console &cin;
extern Console &cout;
extern Console &cerr;

} // namespace IO
} // namespace Msp

#endif
