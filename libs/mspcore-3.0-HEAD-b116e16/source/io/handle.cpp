#include "handle.h"
#include "handle_private.h"

namespace Msp {
namespace IO {

Handle::Handle():
	priv(new Private)
{ }

Handle::Handle(const Handle &other):
	priv(new Private)
{
	priv->handle = other.priv->handle;
}

Handle &Handle::operator=(const Handle &other)
{
	priv->handle = other.priv->handle;
	return *this;
}

Handle::~Handle()
{
	delete priv;
}

Handle::operator const void *() const
{
	return priv->handle!=INVALID_HANDLE_VALUE ? this : 0;
}


Handle::Private::Private():
	handle(INVALID_HANDLE_VALUE)
{ }

Handle::Private &Handle::Private::operator=(PlatformHandle h)
{
	handle = h;
	return *this;
}

} // namespace IO
} // namespace Msp
