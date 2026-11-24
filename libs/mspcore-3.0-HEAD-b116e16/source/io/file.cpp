#include "file.h"
#include "handle_private.h"

using namespace std;

namespace Msp {
namespace IO {

File::File(const string &fn, Mode m, CreateMode cm)
{
	if(!(m&M_RDWR))
		throw invalid_argument("File::File mode");
	if(cm&~(C_CREATE|C_TRUNCATE|C_EXCLUSIVE))
		throw invalid_argument("File::File create");
	if((cm&C_EXCLUSIVE) && (!(cm&C_CREATE) || (cm&C_TRUNCATE)))
		throw invalid_argument("File::File create");

	mode = m;

	platform_init(fn, cm);
}

File::~File()
{
	signal_flush_required.emit();
	sys_close(handle);
}

void File::set_block(bool b)
{
	adjust_mode(mode, M_NONBLOCK, !b);
	sys_set_blocking(handle, b);
}

void File::set_inherit(bool i)
{
	adjust_mode(mode, M_INHERIT, i);
	sys_set_inherit(handle, i);
}

size_t File::do_write(const char *buf, size_t size)
{
	check_access(M_WRITE);

	if(size==0)
		return 0;

#ifdef _WIN32
	if(mode&M_APPEND)
		seek(0, S_END);
#endif

	return sys_write(handle, buf, size);
}

size_t File::do_read(char *buf, size_t size)
{
	check_access(M_READ);

	if(size==0)
		return 0;

	size_t ret = sys_read(handle, buf, size);
	if(ret==0)
		set_eof();

	return ret;
}

SeekOffset File::seek(SeekOffset off, SeekType type)
{
	signal_flush_required.emit();
	off = sys_seek(handle, off, type);
	eof_flag = false;

	return off;
}

SeekOffset File::tell() const
{
	return sys_seek(const_cast<Handle &>(handle), 0, S_CUR);
}

const Handle &File::get_handle(Mode m)
{
	check_access(m);
	return handle;
}


BufferedFile::BufferedFile(const string &fn, Mode m, File::CreateMode cm):
	file(fn, m, cm),
	buffer(file),
	position(0)
{
	mode = m;
	file.signal_end_of_file.connect(sigc::mem_fun(this, &BufferedFile::set_eof));
}

void BufferedFile::set_block(bool b)
{
	file.set_block(b);
}

void BufferedFile::set_inherit(bool)
{
	throw logic_error("BufferedFile::set_inherit");
}

size_t BufferedFile::do_write(const char *buf, size_t size)
{
	size_t ret = buffer.write(buf, size);
	position += ret;
	return ret;
}

size_t BufferedFile::do_read(char *buf, size_t size)
{
	size_t ret = buffer.read(buf, size);
	position += ret;
	return ret;
}

size_t BufferedFile::put(char c)
{
	size_t ret = buffer.put(c);
	position += ret;
	return ret;
}

bool BufferedFile::getline(string &line)
{
	bool ret = buffer.getline(line);
	if(ret)
		position += line.size();
	return ret;
}

int BufferedFile::get()
{
	int ret = buffer.get();
	if(ret>=0)
		++position;
	return ret;
}

const Handle &BufferedFile::get_handle(Mode)
{
	throw logic_error("BufferedFile::get_handle");
}

SeekOffset BufferedFile::seek(SeekOffset offset, SeekType type)
{
	if(type==S_CUR)
	{
		offset += position;
		type = S_BEG;
	}

	signal_flush_required.emit();
	position = file.seek(offset, type);
	eof_flag = false;
	return position;
}

} // namespace IO
} // namespace Msp
