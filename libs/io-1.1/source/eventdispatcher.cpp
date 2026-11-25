/* $Id$

This file is part of libmspio
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#include <msp/time/units.h>
#include "base.h"
#include "eventdispatcher.h"
#include "poll.h"

namespace Msp {
namespace IO {

EventDispatcher::EventDispatcher()
{ }

void EventDispatcher::add(Base &obj)
{
	SlotMap::iterator i=objects.find(&obj);
	if(i==objects.end())
	{
		i=objects.insert(SlotMap::value_type(&obj, Slot(&obj))).first;
		i->second.evch_conn=obj.signal_events_changed.connect(sigc::bind(sigc::mem_fun(this, &EventDispatcher::object_events_changed), &obj));
		i->second.del_conn=obj.signal_deleted.connect(sigc::bind(sigc::mem_fun(this, &EventDispatcher::object_deleted), &obj));

		if(obj.get_events())
			poller.set_object(obj, obj.get_events());
	}
}

void EventDispatcher::remove(Base &obj)
{
	SlotMap::iterator i=objects.find(&obj);
	if(i!=objects.end())
	{
		i->second.evch_conn.disconnect();
		i->second.del_conn.disconnect();
		objects.erase(i);

		poller.set_object(obj, P_NONE);
	}
}

void EventDispatcher::tick()
{
	if(objects.empty())
		return;

	if(poller.poll()>0)
		dispatch();
}

void EventDispatcher::tick(const Time::TimeDelta &dt)
{
	if(objects.empty())
		return;

	if(poller.poll(dt)>0)
		dispatch();
}

void EventDispatcher::object_events_changed(PollEvent ev, Base *obj)
{
	poller.set_object(*obj, ev);
}

void EventDispatcher::object_deleted(Base *obj)
{
	remove(*obj);
}

void EventDispatcher::dispatch()
{
	const Poller::SlotSeq &result=poller.get_result();
	for(Poller::SlotSeq::const_iterator i=result.begin(); i!=result.end(); ++i)
		i->object->event(i->events);
}

} // namespace IO
} // namespace Msp
