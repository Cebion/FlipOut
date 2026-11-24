#ifndef MSP_IO_EVENTDISPATCHER_H_
#define MSP_IO_EVENTDISPATCHER_H_

#include <set>
#include <sigc++/trackable.h>
#include <msp/time/timedelta.h>
#include <msp/time/timer.h>
#include "poll.h"

namespace Msp {
namespace IO {

/**
Put your I/O objects inside one of these to get signaled when something happens
on some of them.
*/
class EventDispatcher
{
private:
	struct Slot: public sigc::trackable
	{
		EventDispatcher &disp;
		EventObject &obj;

		Slot(EventDispatcher &, EventObject &);

		void connect_signals() const;
		void events_changed(PollEvent) const;
		void deleted() const;

		bool operator<(const Slot &o) const { return &obj<&o.obj; }
	};

	Poller poller;
	std::set<Slot> objects;

public:
	void add(EventObject &);
	void remove(EventObject &);

	/** Checks for and dispatches events.  If there are no events available,
	blocks until there are. */
	void tick();

	/** Checks for and dispatches events.  If there are no events available,
	waits at most the specified time before returning. */
	void tick(const Time::TimeDelta &);

	/** Checks for and dispatches events.  If there are no events available,
	waits until the timer's next timeout before returning. */
	void tick(const Time::Timer &);

private:
	void dispatch();
};

} // namespace IO
} // namespace Msp

#endif
