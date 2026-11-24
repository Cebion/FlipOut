/* $Id: source.h 8 2008-08-19 09:41:12Z tdb $

This file is part of libmspal
Copyright © 2008 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_AL_SOURCE_H_
#define MSP_AL_SOURCE_H_

#include <vector>
#include <AL/al.h>
#include "types.h"

namespace Msp {
namespace AL {

class Buffer;

enum SourceState
{
	INITIAL = AL_INITIAL,
	PLAYING = AL_PLAYING,
	PAUSED  = AL_PAUSED,
	STOPPED = AL_STOPPED
};

/**
Represents an audio source in the 3D environment.
*/
class Source
{
private:
	uint id;

public:
	Source();
	~Source();

	void attribute(ALenum, int);
	void attribute(ALenum, float);
	void attribute(ALenum, float, float, float);
	void attribute(ALenum, const float *);
	void get_attribute(ALenum, int *) const;
	void get_attribute(ALenum, float *) const;

	SourceState get_state() const;
	void set_position(float, float, float);
	void set_gain(float);

	void set_rolloff_factor(float);
	void set_buffer(const Buffer &);
	void queue_buffers(const std::vector<const Buffer *> &);
	void queue_buffer(const Buffer &);
	void unqueue_buffers(const std::vector<const Buffer *> &);
	void unqueue_buffer(const Buffer &);
	void clear_buffers();
	unsigned get_buffers_queued() const;
	unsigned get_buffers_processed() const;

	void set_looping(bool);
	void play();
	void pause();
	void stop();
	void rewind();
};

} // namespace AL
} // namespace Msp

#endif
