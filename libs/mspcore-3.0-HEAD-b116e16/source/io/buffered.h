#ifndef MSP_IO_BUFFERED_H_
#define MSP_IO_BUFFERED_H_

#include <sigc++/trackable.h>
#include "base.h"

namespace Msp {
namespace IO {

class Buffered: public Base, public sigc::trackable
{
private:
	Base &below;
	std::size_t buf_size;
	char *buf;
	char *begin;
	char *end;
	Mode cur_op;

public:
	Buffered(Base &, unsigned = 8192);
	~Buffered();

	virtual void set_block(bool);
	virtual void set_inherit(bool);

	void flush();

protected:
	virtual std::size_t do_write(const char *, std::size_t);
	virtual std::size_t do_read(char *, std::size_t);
public:
	virtual std::size_t put(char);

	bool getline(std::string &);
	int get();

	virtual const Handle &get_handle(Mode);

private:
	void set_op(Mode);
public:
	Mode get_current_op() const { return cur_op; }
	std::size_t get_current_size() const;
};

} // namespace IO
} // namespace Msp

#endif
