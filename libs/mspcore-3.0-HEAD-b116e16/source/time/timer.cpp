#include <msp/core/algorithm.h>
#include <msp/core/raii.h>
#include "timer.h"
#include "utils.h"

using namespace std;

namespace Msp {
namespace Time {

Timer::Timer():
	sem(1),
	blocking(false)
{ }

Timer::~Timer()
{
	for(const SlotProxy &s: slots)
		delete s.slot;
}

Timer::Slot &Timer::add(const TimeDelta &td)
{
	Slot *s = new Slot(td);
	MutexLock l(mutex);
	slots.push_back(s);
	push_heap(slots.begin(), slots.end());
	if(blocking)
		sem.signal();
	return *s;
}

Timer::Slot &Timer::add(const TimeStamp &ts)
{
	Slot *s = new Slot(ts);
	MutexLock l(mutex);
	slots.push_back(s);
	push_heap(slots.begin(), slots.end());
	if(blocking)
		sem.signal();
	return *s;
}

void Timer::cancel(Slot &slot)
{
	MutexLock l(mutex);
	auto i = find_member(slots, &slot, &SlotProxy::slot);
	if(i!=slots.end())
	{
		delete i->slot;
		slots.erase(i);
		make_heap(slots.begin(), slots.end());
	}
}

void Timer::tick()
{
	do_tick(-sec);
}

void Timer::tick(const TimeDelta &timeout)
{
	if(timeout<zero)
		throw invalid_argument("Timer::tick");

	do_tick(timeout);
}

void Timer::do_tick(const TimeDelta &timeout)
{
	TimeStamp deadline;
	if(timeout>=zero)
		deadline = now()+timeout;

	Slot *next = 0;
	{
		MutexLock l(mutex);
		while(1)
		{
			TimeStamp stamp;
			TimeStamp t = now();
			if(!slots.empty())
			{
				next = slots.begin()->slot;
				stamp = next->get_timeout();
				if(stamp<=t)
					break;
			}

			if(timeout && (!deadline || t<deadline))
			{
				SetFlag setf(blocking);
				mutex.unlock();
				if(stamp && (!deadline || stamp<deadline))
					sem.wait(stamp-t);
				else if(deadline)
					sem.wait(deadline-t);
				else
					sem.wait();
				mutex.lock();
				// The slots may have changed while waiting so check again
				continue;
			}
			else
				return;
		}

		pop_heap(slots.begin(), slots.end());
		slots.pop_back();
	}

	try
	{
		if(next->signal_timeout.emit() && next->increment())
		{
			MutexLock l(mutex);
			slots.push_back(next);
			push_heap(slots.begin(), slots.end());
		}
		else
			delete next;
	}
	catch(...)
	{
		delete next;
		throw;
	}
}

TimeStamp Timer::get_next_timeout() const
{
	if(slots.empty())
		return TimeStamp();
	return slots.begin()->slot->get_timeout();
}


Timer::Slot::Slot(const TimeDelta &td):
	interval(td),
	timeout(now()+interval)
{ }

Timer::Slot::Slot(const TimeStamp &ts):
	timeout(ts)
{ }

bool Timer::Slot::increment()
{
	if(!interval)
		return false;
	timeout += interval;
	return true;
}


Timer::SlotProxy::SlotProxy(Slot *s):
	slot(s)
{ }

bool Timer::SlotProxy::operator<(const SlotProxy &sp) const
{
	return slot->get_timeout()>sp.slot->get_timeout();
}

} // namespace Time
} // namespace Msp
