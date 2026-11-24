#ifndef MSP_CORE_SEMAPHORE_H_
#define MSP_CORE_SEMAPHORE_H_

#include <msp/time/timedelta.h>
#include "noncopyable.h"

namespace Msp {

class Semaphore: private NonCopyable
{
private:
	struct Private;

	Private *priv;

public:
	Semaphore(unsigned);
	~Semaphore();

	void signal();
	void wait();
	bool wait(const Time::TimeDelta &);
};

}

#endif
