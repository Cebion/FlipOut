#include <windows.h>
#include <algorithm>
#include <msp/core/systemerror.h>
#include "eventreader.h"
#include "handle_private.h"

using namespace std;

namespace Msp {
namespace IO {

struct EventReader::Private
{
	OVERLAPPED overlapped;
	Handle event;
	unsigned buf_size;
	char *buffer;
	unsigned buf_avail;
	char *buf_next;
	bool pending;
	bool eof;
};


EventReader::EventReader(Handle &h, unsigned size):
	handle(h),
	priv(new Private)
{
	memset(&priv->overlapped, 0, sizeof(OVERLAPPED));
	*priv->event = CreateEvent(0, true, false, 0);
	priv->overlapped.hEvent = *priv->event;
	priv->buf_size = size;
	priv->buffer = new char[priv->buf_size];
	priv->buf_avail = 0;
	priv->buf_next = priv->buffer;
	priv->pending = false;
	priv->eof = false;
}

EventReader::~EventReader()
{
	CloseHandle(*priv->event);
	delete[] priv->buffer;
	delete priv;
}

const Handle &EventReader::get_event()
{
	start();
	return priv->event;
}

void EventReader::start()
{
	if(priv->buf_avail || priv->pending)
		return;

	DWORD ret;
	priv->buf_next = priv->buffer;
	if(!ReadFile(*handle, priv->buffer, priv->buf_size, &ret, &priv->overlapped))
	{
		unsigned err = GetLastError();
		if(err==ERROR_BROKEN_PIPE)
			priv->eof = true;
		else if(err==ERROR_IO_PENDING)
			priv->pending = true;
		else
			throw system_error("ReadFile");
	}
	else
	{
		priv->buf_avail = ret;
		SetEvent(*priv->event);
	}
}

void EventReader::wait()
{
	if(!priv->pending)
		return;

	DWORD ret;
	if(!GetOverlappedResult(*handle, &priv->overlapped, &ret, true))
	{
		DWORD err = GetLastError();
		if(err==ERROR_BROKEN_PIPE)
			priv->eof = true;
		else
			throw system_error("GetOverlappedResult");
	}
	else
	{
		priv->buf_avail = ret;
		priv->pending = false;
	}
}

unsigned EventReader::read(char *buf, unsigned len)
{
	if(!priv->buf_avail)
	{
		if(priv->eof)
			return 0;

		// No data in buffer, try to get some
		start();
		wait();
	}

	len = min(len, priv->buf_avail);
	memcpy(buf, priv->buf_next, len);
	priv->buf_next += len;
	priv->buf_avail -= len;

	if(!priv->buf_avail)
	{
		ResetEvent(*priv->event);
		start();
	}

	return len;
}

} // namespace IO
} // namespace Msp
