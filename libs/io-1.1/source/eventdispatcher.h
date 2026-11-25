/* $Id$

This file is part of libmspio
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#ifndef EVENTDISPATCHER_H_
#define EVENTDISPATCHER_H_

#include <sigc++/connection.h>
#include <sigc++/trackable.h>
#include "poll.h"

namespace Msp {
namespace IO {

/**
Put your I/O objects inside one of these to get signaled when something happens
on some of them.
*/
class EventDispatcher: public sigc::trackable
{
public:
	EventDispatcher();
	void add(Base &);
	void remove(Base &);

	/**
	Checks for and dispatches events.  If there are no events available, blocks
	until there are.
	*/
	void tick();

	/**
	Checks for and dispatches events.  If there are no events available, waits
	at most the specified time before returning.
	*/
	void tick(const Time::TimeDelta &);
private:
	struct Slot
	{
		Base *obj;
		sigc::connection evch_conn;
		sigc::connection del_conn;

		Slot(Base *o): obj(o) { }
	};
	typedef std::map<Base *, Slot> SlotMap;

	Poller poller;
	SlotMap objects;

	void object_events_changed(PollEvent, Base *);
	void object_deleted(Base *);
	void dispatch();
};

} // namespace IO
} // namespace Msp

#endif
