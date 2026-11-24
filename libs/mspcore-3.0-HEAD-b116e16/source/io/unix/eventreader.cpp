#include "eventreader.h"

namespace Msp {
namespace IO {

EventReader::EventReader(Handle &h, unsigned):
	handle(h),
	priv(0)
{ }

EventReader::~EventReader()
{ }

const Handle &EventReader::get_event()
{
	return handle;
}

void EventReader::start()
{ }

void EventReader::wait()
{ }

unsigned EventReader::read(char *buf, unsigned len)
{
	return sys_read(handle, buf, len);
}

} // namespace IO
} // namespace Msp
