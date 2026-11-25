/* $Id$

This file is part of libmspio
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#ifndef MSP_IO_POLL_H_
#define MSP_IO_POLL_H_

#ifndef WIN32
#include <poll.h>
#endif
#include <list>
#include <map>
#include <vector>
#include <msp/time/timedelta.h>
#include "types.h"

namespace Msp {
namespace IO {

class Base;

enum PollEvent
{
	P_NONE=0,
	P_INPUT=1,
	P_PRIO=2,
	P_OUTPUT=4,
	P_ERROR=8
};

inline PollEvent operator|(PollEvent e, PollEvent f)
{ return PollEvent(static_cast<int>(e)|static_cast<int>(f)); }

inline PollEvent operator&(PollEvent e, PollEvent f)
{ return PollEvent(static_cast<int>(e)&static_cast<int>(f)); }

inline PollEvent operator~(PollEvent e)
{ return PollEvent(~static_cast<int>(e)); }

class Poller
{
public:
	struct Slot
	{
		Base *object;
		PollEvent events;

		Slot(Base *o, PollEvent e): object(o), events(e) { }
	};
	typedef std::list<Slot> SlotSeq;

	Poller();
	void set_object(Base &, PollEvent);
	int  poll();
	int  poll(const Time::TimeDelta &);
	const SlotSeq &get_result() const { return poll_result; }
private:
	typedef std::map<Base *, Slot> SlotMap;

#ifdef WIN32
	struct pollfd
	{
		Handle fd;
	};
#endif

	SlotMap objects;
	std::vector<pollfd> pfd;
	bool pfd_dirty;
	SlotSeq poll_result;

	void rebuild_pfd();
	int do_poll(int);
};

PollEvent poll(Base &, PollEvent);
PollEvent poll(Base &, PollEvent, const Time::TimeDelta &);

} // namespace IO
} // namespace Msp

#endif
