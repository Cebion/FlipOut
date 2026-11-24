/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MUSIC_H_
#define MUSIC_H_

#include <string>
#include <msp/al/streamer.h>

class Music
{
private:
	std::list<std::string> tracks;
	Msp::AL::Source source;
	Msp::AL::Streamer streamer;
	Msp::AL::Sound *sound;
	std::list<std::string>::iterator current_track;

public:
	Music();
	~Music();

	void set_volume(unsigned);
	void play();
	void stop();
	void tick();
};

#endif
