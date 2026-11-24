#include <windows.h>
#include "mutex.h"
#include "mutex_private.h"

namespace Msp {

Mutex::Mutex():
	priv(new Private)
{
	InitializeCriticalSection(&priv->mutex);
}

Mutex::~Mutex()
{
	DeleteCriticalSection(&priv->mutex);
	delete priv;
}

void Mutex::lock()
{
	EnterCriticalSection(&priv->mutex);
}

bool Mutex::trylock()
{
	return TryEnterCriticalSection(&priv->mutex);
}

void Mutex::unlock()
{
	LeaveCriticalSection(&priv->mutex);
}

} // namespace Msp
