/* $Id$

This file is part of libmspio
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#ifndef WIN32
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#endif
#include <msp/strings/formatter.h>
#include "except.h"
#include "file.h"

using namespace std;

namespace Msp {
namespace IO {

/**
Creates a new file object and opens it.  If the
create flag is true and write access is requested and the file does exist, it
is created.  Otherwise a missing file is an error.

@param   fn  Name of the file to open
@param   m   Open mode
@param   cm  Flags controlling creation of a new file
*/
File::File(const string &fn, Mode m, CreateMode cm)
{
	if(!(m&M_RDWR))
		throw InvalidParameterValue("Invalid read/write mode");
	if(cm&~(C_CREATE|C_TRUNCATE))
		throw InvalidParameterValue("Invalid create mode");

	mode=m;

#ifdef WIN32
	int flags=0;
	int create_flags=OPEN_EXISTING;

	if(mode&M_READ)
		flags|=GENERIC_READ;
	else if(mode&M_WRITE)
	{
		flags|=GENERIC_WRITE;

		switch(static_cast<int>(cm))
		{
		case C_NONE:     create_flags=OPEN_EXISTING; break;
		case C_CREATE:   create_flags=OPEN_ALWAYS; break;
		case C_TRUNCATE: create_flags=TRUNCATE_EXISTING; break;
		case C_CREATE+C_TRUNCATE: create_flags=CREATE_ALWAYS; break;
		}
	}

	handle=CreateFile(fn.c_str(), flags, 0, 0, create_flags, FILE_ATTRIBUTE_NORMAL, 0);
	if(handle==INVALID_HANDLE_VALUE)
	{
		int err=GetLastError();
		if(err==ERROR_FILE_NOT_FOUND)
			throw FileNotFound("Can't find file "+fn, fn);
		else
			throw SystemError(format("Can't open file '%s'", fn), GetLastError());
	}
#else
	int flags=0;
	switch(mode&M_RDWR)
	{
	case M_READ:  flags|=O_RDONLY; break;
	case M_WRITE: flags|=O_WRONLY; break;
	case M_RDWR:  flags|=O_RDWR; break;
	default:;
	}

	if(mode&M_WRITE)
	{
		if(cm&C_CREATE)
			flags|=O_CREAT;
		if(cm&C_TRUNCATE)
			flags|=O_TRUNC;
	}
	if(mode&M_APPEND)
		flags|=O_APPEND;
	if(mode&M_NONBLOCK)
		flags|=O_NONBLOCK;

	handle=::open(fn.c_str(), flags, 0666);
	if(handle==-1)
	{
		int err=errno;
		if(err==ENOENT)
			throw FileNotFound("Can't find file "+fn, fn);
		else
			throw SystemError(format("Can't open file '%s'", fn), err);
	}
#endif

	set_events(P_INPUT);
}

/**
Closes the file.  Any attempt to access the file after this will cause an
exception to be thrown.
*/
void File::close()
{
	if(handle==MSP_IO_INVALID_HANDLE)
		return;

	set_events(P_NONE);

	signal_flush_required.emit();

#ifdef WIN32
	CloseHandle(handle);
#else
	::close(handle);
#endif

	handle=MSP_IO_INVALID_HANDLE;
	signal_closed.emit();
}

/**
Sets the blocking state of the file.  If blocking is disabled, all operations
will return immediately, even if they can't be fully completed.
*/
void File::set_block(bool b)
{
	check_access(M_NONE);

	mode=(mode&~M_NONBLOCK);
	if(b)
		mode=(mode|M_NONBLOCK);
#ifndef WIN32
	int flags=fcntl(handle, F_GETFD);
	fcntl(handle, F_SETFL, (flags&O_NONBLOCK)|(b?0:O_NONBLOCK));
#endif
}

void File::sync()
{
#ifndef WIN32
	signal_flush_required.emit();

	fsync(handle);
#endif
}

/**
Seeks the file to the given byte offset.

@param   off  Offset in bytes
@param   st   Seek type

@return  The resulting offset
*/
int File::seek(int off, SeekType st)
{
	check_access(M_NONE);

	signal_flush_required.emit();

	int type=sys_seek_type(st);
#ifdef WIN32
	DWORD ret=SetFilePointer(handle, off, 0, type);
	if(ret==INVALID_SET_FILE_POINTER)
		throw SystemError("Seek failed", GetLastError());
#else
	int ret=lseek(handle, off, type);
	if(ret==-1)
		throw SystemError("Seek failed", errno);
#endif

	eof_flag=false;

	return ret;
}

/**
Returns the current read/write offset of the file.
*/
int File::tell() const
{
	check_access(M_NONE);

#ifdef WIN32
	DWORD ret=SetFilePointer(handle, 0, 0, FILE_CURRENT);
	if(ret==INVALID_SET_FILE_POINTER)
		throw SystemError("Tell failed", GetLastError());
#else
	int ret=lseek(handle, 0, SEEK_CUR);
	if(ret==-1)
		throw SystemError("Tell failed", errno);
#endif

	return ret;
}

File::~File()
{
	close();
}

void File::check_access(Mode m) const
{
	if(handle==MSP_IO_INVALID_HANDLE)
		throw InvalidState("File is not open");
	if(m==M_READ && !(mode&M_READ))
		throw InvalidState("File is not readable");
	if(m==M_WRITE && !(mode&M_WRITE))
		throw InvalidState("File is not writable");
}

/**
Writes data from a buffer to the file.

@param   buf   Buffer to write from.
@param   size  Length of data to write.

@return  The number of bytes written
*/
unsigned File::do_write(const char *buf, unsigned size)
{
	check_access(M_WRITE);

	if(size==0)
		return 0;

#ifdef WIN32
	if(mode&M_APPEND)
		seek(0, S_END);
	DWORD ret;
	if(WriteFile(handle, buf, size, &ret, 0)==0)
		throw SystemError("Writing to file failed", GetLastError());
#else
	int ret=::write(handle, buf, size);
	if(ret==-1)
	{
		if(errno==EAGAIN)
			return 0;
		else
			throw SystemError("Writing to file failed", errno);
	}
#endif

	return ret;
}

/**
Reads data from the file.

@param   buf   Buffer to read data into.
@param   size  Maximum size of data to read.

@return  The number of bytes read, possibly zero
*/
unsigned File::do_read(char *buf, unsigned size)
{
	check_access(M_READ);

	if(size==0)
		return 0;

#ifdef WIN32
	DWORD ret;
	if(ReadFile(handle, buf, size, &ret, 0)==0)
		throw SystemError("Reading from file failed", GetLastError());
#else
	int ret=::read(handle, buf, size);
	if(ret==-1)
	{
		if(errno==EAGAIN)
			return 0;
		else
			throw SystemError("Reading from file failed", errno);
	}
#endif

	if(ret==0)
	{
		eof_flag=true;
		signal_end_of_file.emit();
	}

	return ret;
}

} // namespace IO
} // namespace Msp
