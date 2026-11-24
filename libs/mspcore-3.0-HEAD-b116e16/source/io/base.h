#ifndef MSP_IO_BASE_H_
#define MSP_IO_BASE_H_

#include <cstddef>
#include <sigc++/sigc++.h>
#include <msp/core/mutex.h>
#include <msp/core/noncopyable.h>
#include "handle.h"
#include "mode.h"
#include "poll.h"

namespace Msp {
namespace IO {

/**
Common interface for all I/O objects.
*/
class Base: private NonCopyable
{
public:
	/** RAII synchronization primitive.  Prevents concurrent access to the
	target object during the lifetime of the Synchronize object. */
	class Synchronize
	{
	private:
		Base &io;

	public:
		Synchronize(Base &);
		~Synchronize();
	};

	/** Emitted when there is no more data to be read. */
	sigc::signal<void> signal_end_of_file;

	/** Emitted when there is a nonlinearity in I/O (such as a file being
	seeked) and any data buffered by upper layers needs to be flushed. */
	sigc::signal<void> signal_flush_required;

	/** Emitted when the object is deleted.  Mainly for use by
	EventDispatcher. */
	sigc::signal<void> signal_deleted;

protected:
	Mode mode;
	bool eof_flag;
	Mutex *mutex;

	Base();
public:
	virtual ~Base();

	/** Sets blocking mode.  When blocking is enabled, most operations won't
	return until they can make progress.  When blocking is disabled, these
	operations may return immediately with a return code indicating that nothing
	was done.

	Blocking is enabled by default. */
	virtual void set_block(bool) = 0;

	/** Sets inheritance mode.  When inheritance is enabled, the file descriptor
	will persist through Process::execute.

	Inheritance is disabled by default. */
	virtual void set_inherit(bool) = 0;

	/** Returns the current mode flags. */
	Mode get_mode() const { return mode; }

protected:
	void check_access(Mode) const;

	virtual std::size_t do_write(const char *, std::size_t) = 0;
	virtual std::size_t do_read(char *, std::size_t) = 0;

public:
	/** Writes data from a buffer.  Subject to blocking.  Returns the number of
	bytes written, which may be zero for a non-blockin operation. */
	std::size_t write(const char *b, std::size_t c) { return do_write(b, c); }

	/** Writes a string.  This is identical to calling
	write(s.data(), s.size()). */
	std::size_t write(const std::string &s) { return do_write(s.data(), s.size()); }

	/** Writes a single character.  This is identical to calling
	write(&c, 1). */
	virtual std::size_t put(char c) { return do_write(&c, 1); }

	/** Reads data into a buffer.  Subject to blocking.  Returns the number of
	bytes read, which may be zero for a non-blocking operation. */
	std::size_t read(char *b, std::size_t c) { return do_read(b, c); }

	/** Reads characters up to the next linefeed or end-of-file.  The linefeed
	is not included in the line.  Returns true if a line was successfully read,
	false otherwise. */
	virtual bool getline(std::string &);

	/** Reads a single character.  Returns -1 if no character was available due
	to end-of-file or non-blocking operation. */
	virtual int get();

protected:
	void set_eof();

public:
	/** Returns the end-of-file flag.  Note that some types of objects won't
	indicate end-of-file until you try to read at least one byte past the actual
	end, while others indicate it when you've read the last byte. */
	bool eof() const { return eof_flag; }

	/** Returns the system-level handle for the object.  Used by Console to
	perform redirections. */
	virtual const Handle &get_handle(Mode) = 0;
};

} // namespace IO
} // namespace Msp

#endif
