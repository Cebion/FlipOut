/* $Id: jukebox.cpp 10 2009-07-26 17:55:52Z tdb $

This file is part of libmspal
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <algorithm>
#include <cstdlib>
#include "jukebox.h"
#include "sound.h"

using namespace std;

namespace Msp {
namespace AL {

Jukebox::Jukebox():
	streamer(source),
	sound(0),
	shuffle(false)
{ }

Jukebox::~Jukebox()
{
	streamer.stop();
	delete sound;
}

void Jukebox::add_track(const string &trk)
{
	bool was_empty=tracks.empty();
	tracks.push_back(trk);
	if(was_empty)
		current_track=tracks.begin();
}

void Jukebox::remove_track(const string &trk)
{
	list<string>::iterator i=find(tracks.begin(), tracks.end(), trk);
	if(i!=tracks.end())
	{
		if(i==current_track)
			next();
		tracks.erase(i);
	}
}

void Jukebox::clear_tracks()
{
	stop();
	tracks.clear();
}

void Jukebox::set_shuffle(bool s)
{
	shuffle=s;
}

void Jukebox::play()
{
	if(tracks.empty() || sound)
		return;

	sound=new AL::Sound;
	sound->open_file(*current_track);
	streamer.play(*sound);
}

void Jukebox::next()
{
	stop();
	if(tracks.size()>1)
	{
		if(shuffle)
		{
			while(1)
			{
				list<string>::iterator i=tracks.begin();
				advance(i, rand()%tracks.size());
				if(i!=current_track)
				{
					current_track = i;
					break;
				}
			}
		}
		else
		{
			++current_track;
			if(current_track==tracks.end())
				current_track=tracks.begin();
		}
	}
	play();
}

void Jukebox::stop()
{
	streamer.stop();
	delete sound;
	sound=0;
}

void Jukebox::tick()
{
	streamer.tick();
	if(sound && sound->eof())
		next();
}

} // namespace AL
} // namespace Msp
