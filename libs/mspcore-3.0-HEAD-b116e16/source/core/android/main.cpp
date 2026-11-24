#include "mainthread.h"

extern "C" void ANativeActivity_onCreate(ANativeActivity *activity, void * /*saved_state*/, size_t /*state_size*/)
{
	static Msp::Android::MainThread *thread = 0;
	if(thread)
		thread->attach_to_activity(activity);
	else
		thread = new Msp::Android::MainThread(activity);
	thread->wait_for_app_created();
}
