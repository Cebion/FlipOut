#ifndef MSP_IO_PIPE_H_
#define MSP_IO_PIPE_H_

#include "eventobject.h"
#include "eventreader.h"
#include "handle.h"

namespace Msp {
namespace IO {

class Pipe: public EventObject
{
private:
	Handle read_handle;
	Handle write_handle;
	EventReader reader;

public:
	Pipe();
private:
	void platform_init();
public:
	~Pipe();

	void set_mode(Mode);
	virtual void set_block(bool);
	virtual void set_inherit(bool);

protected:
	virtual std::size_t do_write(const char *, std::size_t);
	virtual std::size_t do_read(char *, std::size_t);

public:
	virtual const Handle &get_handle(Mode);
	virtual const Handle &get_event_handle() { return reader.get_event(); }
};

} // namespace IO
} // namespace Msp

#endif
