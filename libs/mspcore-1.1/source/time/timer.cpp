/* $Id: timer.cpp 66 2009-09-11 17:32:58Z tdb $

This file is part of libmspcore     
Copyright © 2006, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <algorithm>
#include "timer.h"
#include "utils.h"

using namespace std;

namespace Msp {
namespace Time {

Timer::~Timer()
{
	for(vector<SlotProxy>::iterator i=slots.begin(); i!=slots.end(); ++i)
		delete i->slot;
}

Timer::Slot &Timer::add(const TimeDelta &td)
{
	Slot *s=new Slot(td);
	mutex.lock();
	slots.push_back(s);
	push_heap(slots.begin(), slots.end());
	mutex.unlock();
	sem.signal();
	return *s;
}

Timer::Slot &Timer::add(const TimeStamp &ts)
{
	Slot *s=new Slot(ts);
	{
		MutexLock l(mutex);
		slots.push_back(s);
		push_heap(slots.begin(), slots.end());
	}
	sem.signal();
	return *s;
}

void Timer::cancel(Slot &slot)
{
	MutexLock l(mutex);
	for(vector<SlotProxy>::iterator i=slots.begin(); i!=slots.end(); ++i)
		if(i->slot==&slot)
		{
			delete i->slot;
			slots.erase(i);
			make_heap(slots.begin(), slots.end());
			return;
		}
}

void Timer::tick(bool block)
{
	Slot *next=0;
	{
		MutexLock l(mutex);
		while(1)
		{
			if(slots.empty())
			{
				if(block)
					sem.wait();
				else
					return;
			}

			next=slots.begin()->slot;
			const TimeStamp &stamp=next->get_timeout();
			const TimeStamp t=now();
			if(stamp<=t)
				break;
			else if(block)
				sem.wait(stamp-t);
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
	timeout+=interval;
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
