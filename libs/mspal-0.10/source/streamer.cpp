/* $Id: streamer.cpp 11 2009-08-03 12:28:09Z tdb $

This file is part of libmspal
Copyright © 2008-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "buffer.h"
#include "sound.h"
#include "streamer.h"

using namespace std;

namespace Msp {
namespace AL {

Streamer::Streamer(Source &s):
	src(s),
	snd(0)
{ }

Streamer::~Streamer()
{
	src.stop();
	src.clear_buffers();
	for(list<Buffer *>::iterator i=buffers.begin(); i!=buffers.end(); ++i)
		delete *i;
}

void Streamer::play(Sound &s)
{
	snd=&s;
	tick();
	src.play();
}

void Streamer::stop()
{
	snd=0;
	src.stop();
}

void Streamer::tick()
{
	if(unsigned n=src.get_buffers_processed())
	{
		for(unsigned i=0; i<n; ++i)
		{
			src.unqueue_buffer(*buffers.front());
			delete buffers.front();
			buffers.erase(buffers.begin());
		}
	}

	if(!snd)
		return;

	if(src.get_state()!=PLAYING && src.get_state()!=PAUSED)
		src.play();

	unsigned freq=snd->get_frequency();
	unsigned chunk_size=freq&~0xF;
	unsigned queued=src.get_buffers_queued();
	for(unsigned i=queued; i<4; ++i)
	{
		char data[chunk_size];
		unsigned pos=0;
		while(pos<chunk_size)
		{
			unsigned len=snd->read(data+pos, chunk_size-pos);
			if(len==0)
				break;
			pos+=len;
		}

		if(pos)
		{
			Buffer *buf=new Buffer;
			buf->data(snd->get_format(), data, pos, freq);
			src.queue_buffer(*buf);
			buffers.push_back(buf);
		}
	}

	if(snd->eof())
		snd=0;
}

} // namespace AL
} // namespace Msp
