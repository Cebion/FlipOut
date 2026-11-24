#ifndef MSP_CORE_THREAD_H_
#define MSP_CORE_THREAD_H_

#include <string>
#include "noncopyable.h"

namespace Msp {

/**
Base class for threads.  To create a thread for some task, derive it from this
class and implement the main() function.  Note that threads are not
automatically started upon creation - you must manually call launch() instead.
This is to allow initializing variables of the derived class before the thread
is started.
*/
class Thread: private NonCopyable
{
private:
	struct Private;

	enum State
	{
		PENDING,
		RUNNING,
		FINISHED,
		KILLED,
		JOINED
	};

	Private *priv_;
	std::string name_;
	State state_;

protected:
	Thread(const std::string & = std::string());
public:
	virtual ~Thread();

	const std::string &get_name() const { return name_; }

	/** Indicates whether the thread has finished running. */
	bool is_finished() { return state_>=FINISHED; }

	/** Waits for the thread to exit.  Calling this from the thread will cause a
	deadlock. */
	void join();

	/** Violently terminates the thread.  This should only be used as a last
	resort, as the thread gets no chance to clean up. */
	void kill();

protected:
	/** Starts the thread.  Can only be called once for each Thread instance. */
	void launch();

private:
	void platform_join();
	void platform_kill();
	void platform_launch();
	void platform_setname();

protected:
	virtual void main() = 0;
};

} // namespace Msp

#endif
