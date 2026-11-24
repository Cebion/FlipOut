#include <cerrno>
#include <msp/core/systemerror.h>
#include <poll.h>
#include "eventobject.h"
#include "handle.h"
#include "handle_private.h"
#include "poll.h"
#include "poll_platform.h"

using namespace std;

namespace {

using namespace Msp;
using namespace Msp::IO;

inline short int sys_poll_event(PollEvent event)
{
	int result = 0;

	if(event&~(P_INPUT|P_PRIO|P_OUTPUT))
		throw invalid_argument("sys_poll_event");

	if(event&P_INPUT)
		result |= POLLIN;
	if(event&P_PRIO)
		result |= POLLPRI;
	if(event&P_OUTPUT)
		result |= POLLOUT;

	return result;
}

inline PollEvent poll_event_from_sys(int event)
{
	PollEvent result = P_NONE;

	if(event&POLLIN)
		result = result|P_INPUT;
	if(event&POLLPRI)
		result = result|P_PRIO;
	if(event&POLLOUT)
		result = result|P_OUTPUT;
	if(event&POLLERR)
		result = result|P_ERROR;
	if(event&POLLHUP)
		result = result|P_HANGUP;

	return result;
}

}


namespace Msp {
namespace IO {

void Poller::rebuild_array()
{
	if(objs_changed)
	{
		priv->pfd.clear();
		priv->pfd.reserve(objects.size());

		for(const PolledObject &po: objects)
		{
			pollfd p;
			p.fd = *po.object->get_event_handle();
			p.events = sys_poll_event(po.events);
			priv->pfd.push_back(p);
		}
	}
	else
	{
		for(unsigned i=0; i<objects.size(); ++i)
			priv->pfd[i].events = sys_poll_event(objects[i].events);
	}
}

void Poller::platform_poll(int timeout)
{
	int ret = ::poll(&priv->pfd.front(), priv->pfd.size(), timeout);
	if(ret==-1)
	{
		if(errno==EINTR)
			return;
		else
			throw system_error("poll");
	}

	for(unsigned i=0; (i<objects.size() && ret>0); ++i)
		if(priv->pfd[i].revents)
		{
			poll_result.push_back(PolledObject(objects[i].object, poll_event_from_sys(priv->pfd[i].revents)));
			--ret;
		}
}


PollEvent platform_poll(EventObject &obj, PollEvent pe, int timeout)
{
	pollfd pfd = { *obj.get_event_handle(), sys_poll_event(pe), 0 };

	int ret = ::poll(&pfd, 1, timeout);
	if(ret==-1)
	{
		if(errno==EINTR)
			return P_NONE;
		else
			throw system_error("poll");
	}

	return poll_event_from_sys(pfd.revents);
}

} // namespace IO
} // namespace Msp
