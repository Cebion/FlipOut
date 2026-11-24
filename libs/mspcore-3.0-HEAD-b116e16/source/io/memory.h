#ifndef MSP_IO_MEMORY_H_
#define MSP_IO_MEMORY_H_

#include "seekable.h"

namespace Msp {
namespace IO {

class Memory: public Seekable
{
private:
	char *begin;
	char *end;
	char *pos;

public:
	Memory(char *, std::size_t, Mode = M_RDWR);
	Memory(char *, char *, Mode = M_RDWR);
	Memory(const char *, std::size_t);
	Memory(const char *, const char *);
private:
	void init(char *, char *, Mode);

public:
	virtual void set_block(bool);
	virtual void set_inherit(bool);

private:
	virtual std::size_t do_write(const char *, std::size_t);
	virtual std::size_t do_read(char *, std::size_t);
public:
	virtual std::size_t put(char);
	virtual bool getline(std::string &);
	virtual int get();

	virtual const Handle &get_handle(Mode);

	virtual SeekOffset seek(SeekOffset, SeekType);
	virtual SeekOffset tell() const { return pos-begin; }
};

} // namespace IO
} // namespace Msp

#endif
