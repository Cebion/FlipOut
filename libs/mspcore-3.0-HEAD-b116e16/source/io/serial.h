#ifndef MSP_IO_SERIAL_H_
#define MSP_IO_SERIAL_H_

#include "eventobject.h"
#include "eventreader.h"
#include "handle.h"

namespace Msp {
namespace IO {

class Serial: public EventObject
{
public:
	enum Parity
	{
		NONE,
		EVEN,
		ODD
	};

private:
	struct DeviceState;

	Handle handle;
	EventReader reader;

public:
	Serial(const std::string &);
private:
	void platform_init(const std::string &);
public:
	virtual ~Serial();

private:
	void close();

public:
	virtual void set_block(bool);
	virtual void set_inherit(bool);

	void set_baud_rate(unsigned);
	void set_data_bits(unsigned);
	void set_parity(Parity);
	void set_stop_bits(unsigned);
	void set_parameters(const std::string &);

private:
	virtual unsigned do_write(const char *, unsigned);
	virtual unsigned do_read(char *, unsigned);

public:
	virtual const Handle &get_handle(Mode);
	virtual const Handle &get_event_handle() { return reader.get_event(); }
};

} // namespace IO
} // namespace Msp

#endif
