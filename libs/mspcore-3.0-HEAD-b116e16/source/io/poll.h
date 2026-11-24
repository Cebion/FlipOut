#ifndef MSP_IO_POLL_H_
#define MSP_IO_POLL_H_

#include <map>
#include <vector>
#include <msp/core/noncopyable.h>
#include <msp/time/timedelta.h>

namespace Msp {
namespace IO {

class EventObject;

enum PollEvent
{
	P_NONE = 0,
	P_INPUT = 1,
	P_PRIO = 2,
	P_OUTPUT = 4,
	P_ERROR = 8,
	P_HANGUP = 16
};

inline PollEvent operator|(PollEvent e, PollEvent f)
{ return PollEvent(static_cast<int>(e)|static_cast<int>(f)); }

inline PollEvent operator&(PollEvent e, PollEvent f)
{ return PollEvent(static_cast<int>(e)&static_cast<int>(f)); }

inline PollEvent operator~(PollEvent e)
{ return PollEvent(~static_cast<int>(e)); }


class Poller: private NonCopyable
{
public:
	struct PolledObject
	{
		EventObject *object;
		PollEvent events;

		PolledObject(EventObject *o, PollEvent e): object(o), events(e) { }
	};

private:
	struct Private;

	std::vector<PolledObject> objects;
	Private *priv;
	bool events_changed;
	bool objs_changed;
	std::vector<PolledObject> poll_result;

public:
	Poller();
	~Poller();

	void set_object(EventObject &, PollEvent);
	unsigned poll();
	unsigned poll(const Time::TimeDelta &);
private:
	void rebuild_array();
	unsigned do_poll(int);
	void platform_poll(int);
public:
	const std::vector<PolledObject> &get_result() const { return poll_result; }
};

PollEvent poll(EventObject &, PollEvent);
PollEvent poll(EventObject &, PollEvent, const Time::TimeDelta &);

} // namespace IO
} // namespace Msp

#endif
