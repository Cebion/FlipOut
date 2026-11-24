#ifndef MSP_IO_SEEKABLE_H_
#define MSP_IO_SEEKABLE_H_

#include <cstdint>
#include <stdexcept>
#include "base.h"

namespace Msp {
namespace IO {

class Handle;

typedef std::int64_t SeekOffset;

enum SeekType
{
	S_BEG,
	S_CUR,
	S_END
};


class bad_seek: public std::runtime_error
{
public:
	bad_seek(SeekOffset, SeekType);
	virtual ~bad_seek() throw() { }
};


class Seekable: public Base
{
protected:
	Seekable() { }

public:
	/** Changes the read/write offset.  Returns the new offset. */
	virtual SeekOffset seek(SeekOffset, SeekType) = 0;
	
	/** Returns the current read/write offset. */
	virtual SeekOffset tell() const = 0;
};


SeekOffset sys_seek(Handle &, SeekOffset, SeekType);

} // namespace IO
} // namespace Msp

#endif
