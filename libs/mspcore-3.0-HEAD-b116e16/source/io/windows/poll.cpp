#include <msp/core/systemerror.h>
#include "eventobject.h"
#include "handle.h"
#include "handle_private.h"
#include "poll.h"
#include "poll_platform.h"

using namespace std;

namespace Msp {
namespace IO {

void Poller::rebuild_array()
{
	if(!objs_changed)
		return;

	priv->handles.clear();
	priv->handles.reserve(objects.size());

	for(const PolledObject &po: objects)
		priv->handles.push_back(*po.object->get_event_handle());
}

void Poller::platform_poll(int timeout)
{
	if(timeout<0)
		timeout = INFINITE;

	DWORD ret = WaitForMultipleObjects(priv->handles.size(), &priv->handles.front(), false, timeout);
	if(/*ret>=WAIT_OBJECT_0 &&*/ ret<WAIT_OBJECT_0+priv->handles.size())
	{
		const PolledObject &obj = objects[ret-WAIT_OBJECT_0];
		poll_result.push_back(obj);
	}
	else if(ret==WAIT_FAILED)
		throw system_error("WaitForMultipleObjects");
}


PollEvent platform_poll(EventObject &obj, PollEvent pe, int timeout)
{
	if(timeout<0)
		timeout = INFINITE;

	DWORD ret = WaitForSingleObject(*obj.get_event_handle(), timeout);
	if(ret==WAIT_OBJECT_0)
		return pe;
	else if(ret==WAIT_FAILED)
		throw system_error("WaitForSingleObject");

	return P_NONE;
}

} // namespace IO
} // namespace Msp
