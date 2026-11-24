#ifndef MSP_IO_SLICE_H_
#define MSP_IO_SLICE_H_

#include "seekable.h"

namespace Msp {
namespace IO {

/**
Presents a part of seekable I/O object as if it was a complete object.  This
allows in-place access to embedded resources where the loader expects to
receive a complete file.

When a Slice is created, its read/write offset is placed at the beginning of
its range.  If the offset of the underlying object is changed, the Slice will
restore it before the next access.  This enables multiple Slices to be created
on top of the same object.
*/
class Slice: public Seekable, public sigc::trackable
{
private:
	Seekable &below;
	SeekOffset start_offset;
	SeekOffset length;
	SeekOffset position;
	bool sync_position;

public:
	Slice(Seekable &, SeekOffset, SeekOffset);

	virtual void set_block(bool);
	virtual void set_inherit(bool);

private:
	void flush();

	unsigned prepare_op(unsigned, Mode);
protected:
	virtual std::size_t do_write(const char *, std::size_t);
	virtual std::size_t do_read(char *, std::size_t);

public:
	virtual std::size_t put(char);
	virtual int get();

	virtual const Handle &get_handle(Mode);

	virtual SeekOffset seek(SeekOffset, SeekType);
	virtual SeekOffset tell() const { return position; }
};

} // namespace IO
} // namespace Msp

#endif
