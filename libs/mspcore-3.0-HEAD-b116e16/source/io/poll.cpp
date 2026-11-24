#include <stdexcept>
#include <msp/core/algorithm.h>
#include "eventobject.h"
#include "poll.h"
#include "poll_platform.h"

using namespace std;

namespace Msp {
namespace IO {

Poller::Poller():
	priv(new Private),
	events_changed(false),
	objs_changed(false)
{ }

Poller::~Poller()
{
	delete priv;
}

void Poller::set_object(EventObject &obj, PollEvent ev)
{
	// Verify that the object has an event handle
	if(ev)
		obj.get_event_handle();

	auto i = find_member(objects, &obj, &PolledObject::object);
	if(i!=objects.end())
	{
		if(ev)
			i->events = ev;
		else
		{
			*i = objects.back();
			objects.pop_back();
			objs_changed = true;
		}
		events_changed = true;
		return;
	}

	if(!ev)
		return;

#ifdef _WIN32
	if(objects.size()>=MAXIMUM_WAIT_OBJECTS)
		throw logic_error("Maximum number of wait objects reached");
#endif

	objects.push_back(PolledObject(&obj, ev));
	objs_changed = true;
}

unsigned Poller::poll()
{
	return do_poll(-1);
}

unsigned Poller::poll(const Time::TimeDelta &timeout)
{
	if(timeout<Time::zero)
		throw invalid_argument("Poller::poll");

	return do_poll(static_cast<int>(timeout/Time::msec));
}

unsigned Poller::do_poll(int timeout)
{
	if(objs_changed || events_changed)
	{
		rebuild_array();
		events_changed = false;
		objs_changed = false;
	}

	poll_result.clear();

	platform_poll(timeout);

	return poll_result.size();
}


PollEvent platform_poll(EventObject &, PollEvent, int);

PollEvent poll(EventObject &obj, PollEvent pe)
{
	return platform_poll(obj, pe, -1);
}

PollEvent poll(EventObject &obj, PollEvent pe, const Time::TimeDelta &timeout)
{
	if(timeout<Time::zero)
		throw invalid_argument("poll");

	return platform_poll(obj, pe, static_cast<int>(timeout/Time::msec));
}

} // namespace IO
} // namespace Msp
