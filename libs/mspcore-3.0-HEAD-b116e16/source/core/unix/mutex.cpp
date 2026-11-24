#include <cerrno>
#include <pthread.h>
#include "mutex.h"
#include "mutex_private.h"
#include "systemerror.h"

namespace Msp {

Mutex::Mutex():
	priv(new Private)
{
	pthread_mutex_init(&priv->mutex, 0);
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&priv->mutex);
	delete priv;
}

void Mutex::lock()
{
	if(int err = pthread_mutex_lock(&priv->mutex))
		throw system_error("pthread_mutex_lock", err);
}

bool Mutex::trylock()
{
	int err = pthread_mutex_trylock(&priv->mutex);
	if(err && err!=EBUSY)
		throw system_error("pthread_mutex_trylock", err);
	return !err;
}

void Mutex::unlock()
{
	if(int err = pthread_mutex_unlock(&priv->mutex))
		throw system_error("pthread_mutex_unlock", err);
}

} // namespace Msp
