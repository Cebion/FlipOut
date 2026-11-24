/* $Id: soundscape.h 8 2008-08-19 09:41:12Z tdb $

This file is part of libmspal
Copyright © 2008 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_AL_SOUNDSCAPE_H_
#define MSP_AL_SOUNDSCAPE_H_

#include <list>

namespace Msp {
namespace AL {

class Buffer;
class Source;

/**
A SoundScape offers an easy way to play positional sound effects.
*/
class SoundScape
{
private:
	std::list<Source *> sources;

public:
	~SoundScape();

	void add_source(Source &);
	Source &play(const Buffer &, float, float, float);
	void tick();
};

} // namespace AL
} // namespace Msp

#endif
