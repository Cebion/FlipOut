#include <msp/time/utils.h>
#include "base.h"
#include "eventdispatcher.h"
#include "eventobject.h"
#include "poll.h"

using namespace std;

namespace Msp {
namespace IO {

void EventDispatcher::add(EventObject &obj)
{
	Slot slot(*this, obj);
	auto i = objects.find(slot);
	if(i==objects.end())
	{
		i = objects.insert(slot).first;
		i->connect_signals();

		if(obj.get_events())
			poller.set_object(obj, obj.get_events());
	}
}

void EventDispatcher::remove(EventObject &obj)
{
	auto i = objects.find(Slot(*this, obj));
	if(i!=objects.end())
	{
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

void EventDispatcher::tick(const Time::Timer &timer)
{
	const Time::TimeStamp &timeout = timer.get_next_timeout();
	if(timeout)
		tick(timeout-Time::now());
	else
		tick();
}

void EventDispatcher::dispatch()
{
	for(const Poller::PolledObject &po: poller.get_result())
		if(objects.count(Slot(*this, *po.object)))
			po.object->event(po.events);
}


EventDispatcher::Slot::Slot(EventDispatcher &d, EventObject &o):
	disp(d),
	obj(o)
{ }

void EventDispatcher::Slot::connect_signals() const
{
	obj.signal_events_changed.connect(sigc::mem_fun(this, &Slot::events_changed));
	obj.signal_deleted.connect(sigc::mem_fun(this, &Slot::deleted));
}

void EventDispatcher::Slot::events_changed(PollEvent ev) const
{
	disp.poller.set_object(obj, ev);
}

void EventDispatcher::Slot::deleted() const
{
	disp.remove(obj);
}

} // namespace IO
} // namespace Msp
