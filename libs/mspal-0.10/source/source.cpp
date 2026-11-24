/* $Id: source.cpp 10 2009-07-26 17:55:52Z tdb $

This file is part of libmspal
Copyright © 2008-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "buffer.h"
#include "source.h"

using namespace std;

namespace Msp {
namespace AL {

Source::Source()
{
	alGenSources(1, &id);
}

Source::~Source()
{
	alDeleteSources(1, &id);
}

void Source::attribute(ALenum attr, int v)
{
	alSourcei(id, attr, v);
}

void Source::attribute(ALenum attr, float v)
{
	alSourcef(id, attr, v);
}

void Source::attribute(ALenum attr, float v0, float v1, float v2)
{
	alSource3f(id, attr, v0, v1, v2);
}

void Source::attribute(ALenum attr, const float *v)
{
	alSourcefv(id, attr, v);
}

void Source::get_attribute(ALenum attr, int *v) const
{
	alGetSourcei(id, attr, v);
}

void Source::get_attribute(ALenum attr, float *v) const
{
	alGetSourcef(id, attr, v);
}

SourceState Source::get_state() const
{
	int state;
	get_attribute(AL_SOURCE_STATE, &state);
	return static_cast<SourceState>(state);
}

void Source::set_position(float x, float y, float z)
{
	attribute(AL_POSITION, x, y, z);
}

void Source::set_looping(bool l)
{
	attribute(AL_LOOPING, l);
}

void Source::set_gain(float g)
{
	attribute(AL_GAIN, g);
}

void Source::set_rolloff_factor(float f)
{
	attribute(AL_ROLLOFF_FACTOR, f);
}

void Source::set_buffer(const Buffer &buffer)
{
	attribute(AL_BUFFER, static_cast<int>(buffer.get_id()));
}

void Source::queue_buffers(const vector<const Buffer *> &buffers)
{
	vector<uint> ids;
	ids.reserve(buffers.size());
	for(vector<const Buffer *>::const_iterator i=buffers.begin(); i!=buffers.end(); ++i)
		ids.push_back((*i)->get_id());
	alSourceQueueBuffers(id, ids.size(), &ids.front());
}

void Source::queue_buffer(const Buffer &buffer)
{
	uint bid=buffer.get_id();
	alSourceQueueBuffers(id, 1, &bid);
}

void Source::unqueue_buffers(const vector<const Buffer *> &buffers)
{
	vector<uint> ids;
	ids.reserve(buffers.size());
	for(vector<const Buffer *>::const_iterator i=buffers.begin(); i!=buffers.end(); ++i)
		ids.push_back((*i)->get_id());
	alSourceUnqueueBuffers(id, ids.size(), &ids.front());
}

void Source::unqueue_buffer(const Buffer &buffer)
{
	uint bid=buffer.get_id();
	alSourceUnqueueBuffers(id, 1, &bid);
}

void Source::clear_buffers()
{
	attribute(AL_BUFFER, AL_NONE);
}

unsigned Source::get_buffers_queued() const
{
	int n=0;
	get_attribute(AL_BUFFERS_QUEUED, &n);
	return n;
}

unsigned Source::get_buffers_processed() const
{
	int n=0;
	get_attribute(AL_BUFFERS_PROCESSED, &n);
	return n;
}

void Source::play()
{
	alSourcePlay(id);
}

void Source::pause()
{
	alSourcePause(id);
}

void Source::stop()
{
	alSourceStop(id);
}

void Source::rewind()
{
	alSourceRewind(id);
}

} // namespace AL
} // namespace Msp
