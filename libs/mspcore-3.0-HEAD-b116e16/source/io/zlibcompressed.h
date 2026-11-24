#ifndef MSP_IO_ZLIBCOMPRESSED_H_
#define MSP_IO_ZLIBCOMPRESSED_H_

#include <stdexcept>
#include <string>
#include <sigc++/trackable.h>
#include "base.h"

namespace Msp {
namespace IO {

class zlib_error: public std::runtime_error
{
private:
	int code_;

public:
	zlib_error(const std::string &, int);
	~zlib_error() throw() { }

	int code() const throw() { return code_; }
};

/**
Compresses or decompresses data with zlib.  This class is a filter that
operates on top of another I/O object.

To ensure proper termination of the compressed data stream, the ZlibCompressed
object must be destroyed before the underlying object is closed.
*/
class ZlibCompressed: public Base, public sigc::trackable
{
private:
	struct Private;

	Base &below;
	std::size_t buffer_size;
	unsigned char *in_buffer;
	unsigned char *out_buffer;
	bool stream_end;
	Private *priv;

public:
	/** Creates a zlib de/compression object.  The underlying object must be
	open for reading or writing, not both.  The level parameter determines
	compression quality, ranging from 1 (fastest) to 9 (best compression). */
	ZlibCompressed(Base &, unsigned level = 9);

	/** Creates a zlib de/compression object.  Mode must be either read or
	write, and compatible with the underlying object. */
	ZlibCompressed(Base &, Mode, unsigned level = 9);

private:
	void init(unsigned);

public:
	virtual ~ZlibCompressed();

	virtual void set_block(bool);
	virtual void set_inherit(bool);

	void flush();

protected:
	virtual std::size_t do_write(const char *, std::size_t);

private:
	/** Compresses data and writes it to the underlying object.  Returns true if
	progress was made, false otherwise.  flush_mode can be any of zlib's flush
	modes.  If it is Z_FINISH, false is also returned when the stream has been
	terminated. */
	bool compress_data(int flush_mode);

public:
	virtual std::size_t do_read(char *, std::size_t);

	virtual const Handle &get_handle(Mode);
};

} // namespace IO
} // namespace Msp

#endif
