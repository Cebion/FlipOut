#include <stdexcept>
#include "thread.h"
#include "thread_private.h"

using namespace std;

namespace Msp {

Thread::Thread(const string &name):
	priv_(new Private),
	name_(name),
	state_(PENDING)
{ }

Thread::~Thread()
{
	kill();
	join();
	delete priv_;
}

void Thread::join()
{
	if(state_>=JOINED)
		return;

	platform_join();
	state_ = JOINED;
}

void Thread::kill()
{
	if(state_!=RUNNING)
		return;

	platform_kill();
	state_ = KILLED;
}

void Thread::launch()
{
	if(state_>=RUNNING)
		throw logic_error("already launched");

	platform_launch();
	state_ = RUNNING;
}

ThreadReturn THREAD_CALL Thread::Private::main_wrapper(void *arg)
{
	Thread *thread = reinterpret_cast<Thread *>(arg);
	thread->platform_setname();
	thread->main();
	thread->state_ = FINISHED;
	return 0;
}

} // namespace Msp
