#ifndef MSP_IO_EVENTREADER_H_
#define MSP_IO_EVENTREADER_H_

#include <msp/core/noncopyable.h>
#include "handle.h"

namespace Msp {
namespace IO {

/**
Helper class for reading data in a way that supports events.  For internal use
only.

On Windows, overlapped reads are used.  Data is read to an internal buffer and
handed out from there.  An event object is kept synchronized with the buffer
state to get level-triggered semantics for poll.  Whenever the buffer becomes
empty, a new overlapped read is started.

Unix-based systems can poll the fd directly, so this class reduces to a simple
passthrough to sys_read.
*/
class EventReader: private NonCopyable
{
private:
	struct Private;

	Handle &handle;
	Private *priv;

public:
	EventReader(Handle &, unsigned);
	~EventReader();

	const Handle &get_event();

	void start();
	void wait();
	unsigned read(char *, unsigned);
};

} // namespace IO
} // namespace Msp

#endif
