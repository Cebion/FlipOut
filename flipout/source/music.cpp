/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/al/sound.h>
#include <msp/fs/path.h>
#include <msp/fs/dir.h>
#include <msp/strings/regex.h>
#include "music.h"

using namespace std;
using namespace Msp;

Music::Music():
	tracks(FS::list_filtered("music", "\\.ogg$")),
	streamer(source),
	sound(0)
{
	current_track=tracks.begin();
}

Music::~Music()
{
	stop();
}

void Music::set_volume(unsigned vol)
{
	source.set_gain(vol/100.0f);
}

void Music::play()
{
	if(tracks.empty())
		return;

	sound=new AL::Sound;
	sound->open_file((FS::Path("music")/ *current_track).str());
	streamer.play(*sound);
}

void Music::stop()
{
	streamer.stop();
	delete sound;
	sound=0;
}

void Music::tick()
{
	streamer.tick();

	if(sound && sound->eof())
	{
		delete sound;

		++current_track;
		if(current_track==tracks.end())
			current_track=tracks.begin();

		play();
	}
}
