/* $Id: soundscape.cpp 6 2008-07-08 15:21:39Z tdb $

This file is part of libmspal
Copyright © 2008 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "source.h"
#include "soundscape.h"

using namespace std;

namespace Msp {
namespace AL {

SoundScape::~SoundScape()
{
	for(list<Source *>::iterator i=sources.begin(); i!=sources.end(); ++i)
		delete *i;
}

void SoundScape::add_source(Source &src)
{
	sources.push_back(&src);
}

Source &SoundScape::play(const Buffer &buf, float x, float y, float z)
{
	Source *src=new Source;
	add_source(*src);
	src->set_buffer(buf);
	src->set_position(x, y, z);
	src->play();
	return *src;
}

void SoundScape::tick()
{
	for(list<Source *>::iterator i=sources.begin(); i!=sources.end();)
	{
		if((*i)->get_state()==STOPPED)
		{
			delete *i;
			i=sources.erase(i);
		}
		else
			++i;
	}
}

} // namespace AL
} // namespace Msp
