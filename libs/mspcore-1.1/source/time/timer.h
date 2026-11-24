/* $Id: timer.h 66 2009-09-11 17:32:58Z tdb $

This file is part of libmspcore     
Copyright © 2006, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_TIME_TIMER_H_
#define MSP_TIME_TIMER_H_

#include <vector>
#include <sigc++/sigc++.h>
#include "../core/mutex.h"
#include "../core/semaphore.h"
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
class Timer
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

public:
	~Timer();

	/**
	Adds a timer that will be executed periodically as long as the timeout
	signal hander returns true.
	*/
	Slot &add(const TimeDelta &);

	/**
	Adds a timer that will be executed once at a specific time.  The return
	value of the timeout signal handler is ignored.
	*/
	Slot &add(const TimeStamp &);

	/**
	Cancels a previously added timer.
	*/
	void cancel(Slot &);

	/**
	Checks all timers, executing any that have timed out.  If block is true,
	waits until one times out.

	Note: If there are no active timers when a blocking tick is executed, it
	won't return until a timer is added from another thread.
	*/
	void tick(bool block=true);

	TimeStamp get_next_timeout() const;
};

} // namespace Time
} // namespace Msp

#endif
