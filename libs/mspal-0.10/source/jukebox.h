/* $Id: jukebox.h 10 2009-07-26 17:55:52Z tdb $

This file is part of libmspal
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_AL_JUKEBOX_H_
#define MSP_AL_JUKEBOX_H_

#include <list>
#include <string>
#include "source.h"
#include "streamer.h"

namespace Msp {
namespace AL {

class Sound;

class Jukebox
{
private:
	Source source;
	Streamer streamer;
	Sound *sound;
	std::list<std::string> tracks;
	std::list<std::string>::iterator current_track;
	bool shuffle;

public:
	Jukebox();
	~Jukebox();

	Source &get_source() { return source; }
	void add_track(const std::string &);
	void remove_track(const std::string &);
	void clear_tracks();
	void set_shuffle(bool);

	void play();
	void next();
	void stop();
	void tick();
};

} // namespace AL
} // namespace Msp

#endif
