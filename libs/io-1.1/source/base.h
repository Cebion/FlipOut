/* $Id$

This file is part of libmspio
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#ifndef MSP_IO_BASE_H_
#define MSP_IO_BASE_H_

#include <sigc++/sigc++.h>
#include "mode.h"
#include "poll.h"
#include "types.h"

namespace Msp {
namespace IO {

/**
Common interface for all I/O objects.

A derived class must call set_events(P_NONE) before it is destroyed to avoid
leaving stale pointers in an EventDispatcher.
*/
class Base
{
public:
	/**
	Emitted when there is data available for reading.  If all data is not read,
	the signal is emitted again immediately.
	*/
	sigc::signal<void> signal_data_available;

	/**
	Emitted when there is no more data to be read.
	*/
	sigc::signal<void> signal_end_of_file;

	/**
	Emitted when there is a nonlinearity in I/O (such as a file being seeked)
	and any data buffered by upper layers needs to be flushed.
	*/
	sigc::signal<void> signal_flush_required;

	/**
	Emitted when the I/O object has closed.
	*/
	sigc::signal<void> signal_closed;

	/**
	Emitted when the mask of interesting events changes.  Mainly for use by
	EventDispatcher.
	*/
	sigc::signal<void, PollEvent> signal_events_changed;

	/**
	Emitted when the object is deleted.  Mainly for use by EventDispatcher.
	*/
	sigc::signal<void> signal_deleted;

	/**
	Sets blocking mode.  When blocking is enabled, most operations won't return
	until they can make progress.  When blocking is disabled, these operations
	may return immediately with a return code indicating that nothing was done.

	Blocking is enabled by default.
	*/
	virtual void set_block(bool) { }

	/**
	Returns the current mode flags.
	*/
	Mode get_mode() const { return mode; }

	/**
	Writes data from a buffer.  Subject to blocking.

	@param   b  Buffer to write from
	@param   c  Number of bytes to write

	@return  Number of bytes written
	*/
	unsigned write(const char *b, unsigned c) { return do_write(b, c); }

	/**
	Writes a string.  This is identical to calling write(s.data(), s.size()).
	*/
	unsigned write(const std::string &s) { return do_write(s.data(), s.size()); }

	/**
	Writes a single character.  This is identical to calling write(&c, 1).
	*/
	virtual unsigned put(char c) { return do_write(&c, 1); }

	/**
	Reads data into a buffer.  Subject to blocking.

	@param   b  Buffer to read into
	@param   c  Maximum number of bytes to read

	@return  Number of bytes read
	*/
	unsigned read(char *b, unsigned c) { return do_read(b, c); }

	/**
	Reads characters up to the next linefeed or end-of-file.  The linefeed is not
	included in the line.

	@param   line  String to put the characters into

	@return  Whether anything was read
	*/
	virtual bool getline(std::string &);

	/**
	Reads a single character.

	@return  A character, or -1 if none were available
	*/
	virtual int get();

	/**
	Returns the end-of-file flag.
	*/
	bool eof() const { return eof_flag; }

	/**
	Returns a mask of the currently interesting events.  Used by EventDispatcher.
	*/
	PollEvent get_events() const { return events; }

	/**
	Returns a handle for polling.  Should throw if the object does not have an
	event handle.
	*/
	virtual Handle get_event_handle() =0;

	/**
	Notifies the object of an event.  Used by EventDispatcher.
	*/
	void event(PollEvent);

	virtual ~Base();
protected:
	Mode mode;
	PollEvent events;
	bool eof_flag;

	Base();
	void             set_events(PollEvent);
	virtual void     on_event(PollEvent) { }
	virtual unsigned do_write(const char *, unsigned) =0;
	virtual unsigned do_read(char *, unsigned) =0;
private:
	Base(const Base &);
	Base &operator=(const Base &);
};

} // namespace IO
} // namespace Msp

#endif
