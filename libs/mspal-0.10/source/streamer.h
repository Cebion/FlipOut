/* $Id: streamer.h 8 2008-08-19 09:41:12Z tdb $

This file is part of libmspal
Copyright © 2008 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_AL_STREAMER_H_
#define MSP_AL_STREAMER_H_

#include <list>
#include "source.h"

namespace Msp {
namespace AL {

class Sound;
class Buffer;

/**
A streamer transfers data from a Sound to a Source.  It is permanently attached
to the same Source, but can play multiple Sounds during its lifetime.  A single
Sound can only be played by one Streamer at a time.
*/
class Streamer
{
private:
	Source &src;
	Sound  *snd;
	std::list<Buffer *> buffers;

public:
	Streamer(Source &);
	~Streamer();

	void play(Sound &);
	void stop();
	void tick();
};

} // namespace AL
} // namespace Msp

#endif
