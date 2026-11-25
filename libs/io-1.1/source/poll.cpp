/* $Id$

This file is part of libmspio
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#include <errno.h>
#include <msp/strings/formatter.h>
#include <msp/time/units.h>
#include "except.h"
#include "base.h"
#include "poll.h"

namespace {

using namespace Msp;
using namespace Msp::IO;

inline int sys_poll_event(PollEvent event)
{
	int result=0;

	if(event&~(P_INPUT|P_PRIO|P_OUTPUT))
		throw InvalidParameterValue("Invalid poll events");

#ifndef WIN32
	if(event&P_INPUT)
		result|=POLLIN;
	if(event&P_PRIO)
		result|=POLLPRI;
	if(event&P_OUTPUT)
		result|=POLLOUT;
#endif

	return result;
}

inline PollEvent poll_event_from_sys(int event)
{
	PollEvent result=P_NONE;

#ifdef WIN32
	// Stop the compiler from complaining about unused parameter
	event=event;
#else
	if(event&POLLIN)
		result=result|P_INPUT;
	if(event&POLLPRI)
		result=result|P_PRIO;
	if(event&POLLOUT)
		result=result|P_OUTPUT;
	if(event&POLLERR)
		result=result|P_ERROR;
#endif

	return result;
}

inline PollEvent do_poll(Base &obj, PollEvent pe, int timeout)
{
#ifdef WIN32
	if(timeout<0)
		timeout=INFINITE;

	DWORD ret=WaitForSingleObject(obj.get_event_handle(), timeout);
	if(ret==WAIT_OBJECT_0)
		return pe;
	else if(ret==WAIT_FAILED)
		throw SystemError("Poll failed", GetLastError());

	return P_NONE;
#else
	pollfd pfd={obj.get_event_handle(), sys_poll_event(pe), 0};

	int ret=::poll(&pfd, 1, timeout);
	if(ret==-1)
	{
		if(errno==EINTR)
			return P_NONE;
		else
			throw SystemError("Poll failed", errno);
	}

	return poll_event_from_sys(pfd.revents);
#endif
}

}

namespace Msp {
namespace IO {

Poller::Poller():
	pfd_dirty(false)
{ }

void Poller::set_object(Base &obj, PollEvent ev)
{
	// Verify that the object has an event handle
	if(ev)
		obj.get_event_handle();

	SlotMap::iterator i=objects.find(&obj);
	if(i!=objects.end())
	{
		if(ev)
			i->second.events=ev;
		else
			objects.erase(i);

		pfd_dirty=true;
	}
	else if(ev)
	{
#ifdef WIN32
		if(objects.size()>=MAXIMUM_WAIT_OBJECTS)
			throw InvalidState("Maximum number of wait objects reached");
#endif
		objects.insert(SlotMap::value_type(&obj, Slot(&obj, ev)));

		pfd_dirty=true;
	}
}

int Poller::poll()
{
	return do_poll(-1);
}

int Poller::poll(const Time::TimeDelta &timeout)
{
	if(timeout<Time::zero)
		throw InvalidParameterValue("Invalid timeout");

	return do_poll(static_cast<int>(timeout/Time::msec));
}

void Poller::rebuild_pfd()
{
	pfd.clear();

	pollfd p;

	for(SlotMap::iterator i=objects.begin(); i!=objects.end(); ++i)
	{
		p.fd=i->second.object->get_event_handle();
#ifndef WIN32
		p.events=sys_poll_event(i->second.events);
#endif
		pfd.push_back(p);
	}

	pfd_dirty=false;
}

int Poller::do_poll(int timeout)
{
	if(pfd_dirty)
		rebuild_pfd();

	poll_result.clear();

#ifdef WIN32
	if(timeout<0)
		timeout=INFINITE;

	DWORD ret=WaitForMultipleObjects(pfd.size(), &pfd.front().fd, false, timeout);
	if(/*ret>=WAIT_OBJECT_0 &&*/ ret<WAIT_OBJECT_0+pfd.size())
	{
		SlotMap::iterator i=objects.begin();
		advance(i, ret-WAIT_OBJECT_0);
		poll_result.push_back(Slot(i->second.object, i->second.events));

		return 1;
	}
	else if(ret==WAIT_FAILED)
		throw SystemError("Poll failed", GetLastError());

	return 0;
#else
	int ret=::poll(&pfd.front(), pfd.size(), timeout);
	if(ret==-1)
	{
		if(errno==EINTR)
			return 0;
		else
			throw SystemError("Poll failed", errno);
	}

	int n=ret;
	SlotMap::iterator j=objects.begin();
	for(std::vector<pollfd>::iterator i=pfd.begin(); (i!=pfd.end() && n>0); ++i,++j)
		if(i->revents)
		{
			poll_result.push_back(Slot(j->second.object, poll_event_from_sys(i->revents)));
			--n;
		}

	return ret;
#endif
}


PollEvent poll(Base &obj, PollEvent pe)
{
	return do_poll(obj, pe, -1);
}

PollEvent poll(Base &obj, PollEvent pe, const Time::TimeDelta &timeout)
{
	if(timeout<Time::zero)
		throw InvalidParameterValue("Invalid timeout");

	return do_poll(obj, pe, static_cast<int>(timeout/Time::msec));
}

} // namespace IO
} // namespace Msp
