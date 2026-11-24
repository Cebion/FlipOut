#ifndef MSP_IO_FILE_H_
#define MSP_IO_FILE_H_

#include <stdexcept>
#include <string>
#include "buffered.h"
#include "filtered.h"
#include "handle.h"
#include "seekable.h"

namespace Msp {
namespace IO {

class file_not_found: public std::runtime_error
{
public:
	file_not_found(const std::string &fn): std::runtime_error(fn) { }
	~file_not_found() throw() { }
};

class file_already_exists: public std::runtime_error
{
public:
	file_already_exists(const std::string &fn): std::runtime_error(fn) { }
	~file_already_exists() throw() { }
};


/**
A class for reading and writing files.

Non-blocking mode is not supported on Win32.
*/
class File: public Seekable
{
public:
	enum CreateMode
	{
		C_NONE = 0,
		C_CREATE = 1,
		C_TRUNCATE = 2,
		C_OVERWRITE = C_CREATE+C_TRUNCATE,
		C_EXCLUSIVE = 4,
		C_NEW = C_CREATE+C_EXCLUSIVE
	};

private:
	Handle handle;

public:
	/** Creates a new file object and opens it.  If the create flag is set and
	write access is requested and the file does exist, it is created.  Otherwise
	a missing file is an error. */
	File(const std::string &, Mode = M_READ, CreateMode = C_OVERWRITE);
private:
	void platform_init(const std::string &, CreateMode);
public:
	virtual ~File();

	virtual void set_block(bool);
	virtual void set_inherit(bool);

protected:
	virtual std::size_t do_write(const char *, std::size_t);
	virtual std::size_t do_read(char *, std::size_t);

public:
	virtual void sync();

	virtual SeekOffset seek(SeekOffset, SeekType);
	virtual SeekOffset tell() const;

	virtual const Handle &get_handle(Mode);
};

inline File::CreateMode operator|(File::CreateMode m, File::CreateMode n)
{ return File::CreateMode(static_cast<int>(m)|static_cast<int>(n)); }

inline File::CreateMode operator&(File::CreateMode m, File::CreateMode n)
{ return File::CreateMode(static_cast<int>(m)&static_cast<int>(n)); }

inline File::CreateMode operator~(File::CreateMode m)
{ return File::CreateMode(~static_cast<int>(m)); }


class BufferedFile: public Seekable
{
private:
	File file;
	Buffered buffer;
	SeekOffset position;

public:
	BufferedFile(const std::string &, Mode = M_READ, File::CreateMode = File::C_OVERWRITE);

	virtual void set_block(bool);
	virtual void set_inherit(bool);

protected:
	virtual std::size_t do_write(const char *, std::size_t);
	virtual std::size_t do_read(char *, std::size_t);
public:
	virtual std::size_t put(char);

	virtual bool getline(std::string &);
	virtual int get();

	virtual const Handle &get_handle(Mode);

	virtual SeekOffset seek(SeekOffset, SeekType);
	virtual SeekOffset tell() const { return position; }
};

} // namespace IO
} // namespace Msp

#endif
