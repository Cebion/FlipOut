#ifndef MSP_IO_EVENTOBJECT_H_
#define MSP_IO_EVENTOBJECT_H_

#include "base.h"

namespace Msp {
namespace IO {

class Handle;

/**
Interface class for objects that can provide event-based I/O.  These objects
can be fed to the various poll functions in poll.h, or added to an
EventDispatcher to generate event signals.
*/
class EventObject: public Base
{
public:
	/** Emitted when there is data available for reading.  If all data is not
	read, the signal is emitted again immediately. */
	sigc::signal<void> signal_data_available;

	/** Emitted when the mask of interesting events changes.  Mainly for use by
	EventDispatcher. */
	sigc::signal<void, PollEvent> signal_events_changed;

private:
	PollEvent events;

protected:
	EventObject();
	virtual ~EventObject();

	void set_events(PollEvent);
public:
	/** Returns a mask of the currently interesting events.  Used by
	EventDispatcher. */
	PollEvent get_events() const { return events; }

	/** Returns a handle for polling. */
	virtual const Handle &get_event_handle() = 0;

	/** Notifies the object of an event.  Used by EventDispatcher. */
	void event(PollEvent);

protected:
	/** Called when an event occurs.  Derived classes can implement this to
	process events. */
	virtual void on_event(PollEvent) { }
};

} // namespace IO
} // namespace Msp

#endif
