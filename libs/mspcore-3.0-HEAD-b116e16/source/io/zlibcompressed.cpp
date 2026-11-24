#ifdef WITH_ZLIB
#include <zlib.h>
#endif
#include "zlibcompressed.h"

using namespace std;

namespace Msp {
namespace IO {

zlib_error::zlib_error(const string &w, int c):
#ifdef WITH_ZLIB
	runtime_error(w+": "+zError(c)),
#else
	runtime_error(w),
#endif
	code_(c)
{ }


struct ZlibCompressed::Private
{
#ifdef WITH_ZLIB
	z_stream stream;
#endif

	Private();
};

ZlibCompressed::Private::Private()
{
#ifdef WITH_ZLIB
	stream.zalloc = 0;
	stream.zfree = 0;
	stream.opaque = 0;
#endif
}


ZlibCompressed::ZlibCompressed(Base &b, unsigned level):
	below(b)
{
	mode = below.get_mode()&M_RDWR;
	if(mode!=M_READ && mode!=M_WRITE)
		throw invalid_access(mode);

	init(level);
}

ZlibCompressed::ZlibCompressed(Base &b, Mode m, unsigned level):
	below(b)
{
	mode = m&below.get_mode()&M_RDWR;
	if(mode!=M_READ && mode!=M_WRITE)
		throw invalid_access(m);

	init(level);
}

void ZlibCompressed::init(unsigned level)
{
#ifdef WITH_ZLIB
	buffer_size = 1024;
	in_buffer = 0;
	out_buffer = 0;
	stream_end = false;
	priv = new Private;

	if(mode==M_WRITE)
	{
		int ret = deflateInit(&priv->stream, level);
		if(ret!=Z_OK)
			throw zlib_error("deflateInit", ret);
	}
	else
	{
		int ret = inflateInit(&priv->stream);
		if(ret!=Z_OK)
			throw zlib_error("inflateInit", ret);
	}

	in_buffer = new unsigned char[buffer_size];
	out_buffer = new unsigned char[buffer_size];

	priv->stream.next_in = in_buffer;
	priv->stream.avail_in = 0;
	priv->stream.next_out = out_buffer;
	priv->stream.avail_out = buffer_size;

	below.signal_flush_required.connect(sigc::mem_fun(this, &ZlibCompressed::flush));
#else
	(void)buffer_size;
	(void)stream_end;
	(void)level;
	throw zlib_error("unsupported", -1);
#endif
}

ZlibCompressed::~ZlibCompressed()
{
#ifdef WITH_ZLIB
	if(mode==M_WRITE)
	{
		while(compress_data(Z_FINISH)) ;
		deflateEnd(&priv->stream);
	}
	else
		inflateEnd(&priv->stream);
#endif

	delete[] in_buffer;
	delete[] out_buffer;
	delete priv;
}

void ZlibCompressed::set_block(bool)
{
	throw logic_error("ZlibCompressed::set_block");
}

void ZlibCompressed::set_inherit(bool)
{
	throw logic_error("ZlibCompressed::set_inherit");
}

void ZlibCompressed::flush()
{
#ifdef WITH_ZLIB
	if(mode==M_WRITE)
	{
		while(1)
		{
			if(!compress_data(Z_SYNC_FLUSH))
				break;

			// The flush is done when all input data has been consumed
			if(!priv->stream.avail_in)
				break;
		}
	}
#endif
}

size_t ZlibCompressed::do_write(const char *data, size_t size)
{
	check_access(M_WRITE);

	size_t processed = 0;
#ifdef WITH_ZLIB
	while(processed<size)
	{
		size_t free_in = (in_buffer+buffer_size-priv->stream.next_in);
		if(free_in<size && priv->stream.next_in>in_buffer)
		{
			// Not all of the data fits in the buffer, so make some more room
			copy(priv->stream.next_in, priv->stream.next_in+priv->stream.avail_in, in_buffer);
			priv->stream.next_in = in_buffer;
			free_in = buffer_size-priv->stream.avail_in;
		}

		if(free_in)
		{
			// Copy as much data into the input buffer as possible
			size_t len = min(free_in, size-processed);
			copy(data+processed, data+processed+len, priv->stream.next_in+priv->stream.avail_in);
			priv->stream.avail_in += len;
			processed += len;
		}

		bool stalled = false;
		while(priv->stream.avail_in && !stalled)
			stalled = !compress_data(Z_NO_FLUSH);
		if(stalled)
			break;
	}
#else
	(void)data;
	(void)size;
#endif

	return processed;
}

bool ZlibCompressed::compress_data(int flush_mode)
{
#ifdef WITH_ZLIB
	bool can_deflate = ((priv->stream.avail_in || flush_mode) && priv->stream.avail_out);
	bool finished = false;
	if(can_deflate)
	{
		int ret = deflate(&priv->stream, flush_mode);
		if(flush_mode==Z_FINISH && ret==Z_STREAM_END)
			finished = true;
		else if(ret!=Z_OK)
			throw zlib_error("deflate", ret);
	}

	// Write compressed data into the underlying object
	size_t len = 0;
	if(priv->stream.next_out>out_buffer)
		len = below.write(reinterpret_cast<char *>(out_buffer), priv->stream.next_out-out_buffer);
	if(len>0)
	{
		if(len<static_cast<size_t>(priv->stream.next_out-out_buffer))
			copy(out_buffer+len, priv->stream.next_out, out_buffer);
		priv->stream.avail_out += len;
		priv->stream.next_out -= len;
	}
	else if(!can_deflate)
		// We weren't able to do anything
		return false;

	return !finished;
#else
	(void)flush_mode;
	return false;
#endif
}

size_t ZlibCompressed::do_read(char *data, size_t size)
{
	check_access(M_READ);

	size_t processed = 0;
#ifdef WITH_ZLIB
	while(processed<size)
	{
		if(priv->stream.next_out>out_buffer)
		{
			// We have some pending output, give it out first
			size_t len = min<size_t>(priv->stream.next_out-out_buffer, size-processed);

			copy(out_buffer, out_buffer+len, data+processed);
			processed += len;

			if(len<static_cast<size_t>(priv->stream.next_out-out_buffer))
				copy(out_buffer+len, priv->stream.next_out, out_buffer);
			priv->stream.next_out -= len;
			priv->stream.avail_out += len;

			continue;
		}

		bool need_more_input = !priv->stream.avail_in;
		if(priv->stream.avail_in)
		{
			int ret = inflate(&priv->stream, Z_NO_FLUSH);
			if(ret==Z_STREAM_END)
				stream_end = true;
			else if(ret!=Z_OK)
				throw zlib_error("inflate", ret);
			need_more_input = (priv->stream.next_out==out_buffer);
		}

		if(need_more_input)
		{
			if(stream_end)
				break;

			if(priv->stream.next_in>in_buffer)
				copy(priv->stream.next_in, priv->stream.next_in+priv->stream.avail_in, in_buffer);
			priv->stream.next_in = in_buffer;

			size_t len = below.read(reinterpret_cast<char *>(priv->stream.next_in), in_buffer+buffer_size-priv->stream.next_in);
			priv->stream.avail_in += len;
			if(!len && below.eof())
				stream_end = true;
		}
	}

	if(size>0 && processed==0 && stream_end)
		set_eof();
#else
	(void)data;
	(void)size;
#endif

	return processed;
}

const Handle &ZlibCompressed::get_handle(Mode)
{
	throw logic_error("ZlibCompressed::get_handle");
}

} // namespace IO
} // namespace Msp
