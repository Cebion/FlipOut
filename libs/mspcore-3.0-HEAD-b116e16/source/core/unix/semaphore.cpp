#include <sys/time.h>
#include <cerrno>
#include <msp/core/systemerror.h>
#include <msp/time/rawtime_private.h>
#include <msp/time/timestamp.h>
#include <msp/time/utils.h>
#include "mutex.h"
#include "mutex_private.h"
#include "semaphore.h"

namespace Msp {

struct Semaphore::Private
{
	Mutex mutex;
	pthread_cond_t cond;
	unsigned limit;
	unsigned count;
};


Semaphore::Semaphore(unsigned limit):
	priv(new Private)
{
	pthread_cond_init(&priv->cond, 0);
	priv->limit = limit;
	priv->count = 0;
}

Semaphore::~Semaphore()
{
	pthread_cond_destroy(&priv->cond);
	delete priv;
}

void Semaphore::signal()
{
	MutexLock mlock(priv->mutex);
	if(priv->count<priv->limit)
		++priv->count;
	if(int err = pthread_cond_signal(&priv->cond))
		throw system_error("pthread_cond_signal", err);
}

void Semaphore::wait()
{ 
	MutexLock mlock(priv->mutex);
	while(!priv->count)
		if(int err = pthread_cond_wait(&priv->cond, &priv->mutex.priv->mutex))
			throw system_error("pthread_cond_wait", err);
	--priv->count;
}

bool Semaphore::wait(const Time::TimeDelta &d)
{
	timespec timeout = Time::rawtime_to_timespec((Time::now()+d).raw());

	int err = pthread_cond_timedwait(&priv->cond, &priv->mutex.priv->mutex, &timeout);
	if(err && err!=ETIMEDOUT)
		throw system_error("pthread_cond_timedwait", err);
	return err==0;
}

} // namespace Msp
