/* $Id$

This file is part of libmspio
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#ifndef MSP_IO_FILE_H_
#define MSP_IO_FILE_H_

#include <string>
#include "base.h"
#include "buffered.h"
#include "filtered.h"
#include "seek.h"

namespace Msp {
namespace IO {

/**
A class for reading and writing files.

Non-blocking mode is not supported on Win32.
*/
class File: public Base
{
public:
	enum CreateMode
	{
		C_NONE=0,
		C_CREATE=1,
		C_TRUNCATE=2
	};

	File(const std::string &, Mode=M_READ, CreateMode =CreateMode(C_CREATE+C_TRUNCATE));

	void close();

	void set_block(bool);

	virtual void sync();

	virtual int  seek(int, SeekType);
	virtual int  tell() const;

	virtual Handle get_event_handle() { return handle; }

	virtual ~File();
private:
	Handle handle;

	void              check_access(Mode) const;
protected:
	virtual unsigned  do_write(const char *, unsigned);
	virtual unsigned  do_read(char *, unsigned);
};

inline File::CreateMode operator|(File::CreateMode m, File::CreateMode n)
{ return File::CreateMode(static_cast<int>(m)|static_cast<int>(n)); }

inline File::CreateMode operator&(File::CreateMode m, File::CreateMode n)
{ return File::CreateMode(static_cast<int>(m)&static_cast<int>(n)); }

inline File::CreateMode operator~(File::CreateMode m)
{ return File::CreateMode(~static_cast<int>(m)); }

typedef Filtered<File, Buffered> BufferedFile;

} // namespace IO
} // namespace Msp

#endif
