#ifndef MSP_TIME_TIMER_H_
#define MSP_TIME_TIMER_H_

#include <vector>
#include <sigc++/sigc++.h>
#include <msp/core/mutex.h>
#include <msp/core/noncopyable.h>
#include <msp/core/semaphore.h>
#include "timedelta.h"
#include "timestamp.h"

namespace Msp {
namespace Time {

/**
A class for executing functions in a deferred or periodical fashion.  The add a
timer, use one of the add functions and connect a functor to the timeout signal
of the returned slot.

This class is thread-safe, to allow running timers in a separate thread.
*/
class Timer: private NonCopyable
{
public:
	class Slot
	{
	public:
		sigc::signal<bool> signal_timeout;
	
	private:
		TimeDelta interval;
		TimeStamp timeout;
	
	public:
		Slot(const TimeDelta &);
		Slot(const TimeStamp &);
		const TimeStamp &get_timeout() const { return timeout; }
		bool increment();
	};

private:
	struct SlotProxy
	{
		Slot *slot;

		SlotProxy(Slot *);
		bool operator<(const SlotProxy &) const;
	};

	std::vector<SlotProxy> slots;
	Semaphore sem;
	Mutex mutex;
	bool blocking;

public:
	Timer();
	~Timer();

	/** Adds a timer that will be executed periodically as long as the timeout
	signal hander returns true. */
	Slot &add(const TimeDelta &);

	/** Adds a timer that will be executed once at a specific time.  The return
	value of the timeout signal handler is ignored. */
	Slot &add(const TimeStamp &);

	/** Cancels a previously added timer. */
	void cancel(Slot &);

	/** Waits until a timer expires, then executes it.  If no timers have been
	set, blocks until one is added from another thread. */
	void tick();

	/** Waits until a timer expires but at most the specified amount of time.
	If a timer did expire before the timeout, it is executed. */
	void tick(const TimeDelta &);

private:
	void do_tick(const TimeDelta &);

public:
	TimeStamp get_next_timeout() const;
};

} // namespace Time
} // namespace Msp

#endif
