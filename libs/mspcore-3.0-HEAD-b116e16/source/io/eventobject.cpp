#include "eventobject.h"

namespace Msp {
namespace IO {

EventObject::EventObject():
	events(P_NONE)
{ }

EventObject::~EventObject()
{ }

void EventObject::set_events(PollEvent e)
{
	events = e;
	signal_events_changed.emit(events);
}

void EventObject::event(PollEvent ev)
{
	if(ev&P_INPUT)
		signal_data_available.emit();
	if(ev&P_HANGUP)
		signal_end_of_file.emit();

	on_event(ev);
}
} // namespace IO
} // namespace Msp
