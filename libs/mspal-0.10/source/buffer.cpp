/* $Id: buffer.cpp 7 2008-07-09 16:06:09Z tdb $

This file is part of libmspal
Copyright © 2008 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "buffer.h"
#include "sound.h"

using namespace std;

namespace Msp {
namespace AL {

Buffer::Buffer()
{
	alGenBuffers(1, &id);
}

Buffer::~Buffer()
{
	alDeleteBuffers(1, &id);
}

void Buffer::data(Format fmt, const void *dt, sizei size, sizei freq)
{
	alBufferData(id, fmt, dt, size, freq);
}

void Buffer::data(const Sound &snd)
{
	data(snd.get_format(), snd.get_data(), snd.get_size(), snd.get_frequency());
}

void Buffer::load_data(const string &fn)
{
	Sound sound;
	sound.load_file(fn);

	data(sound);
}


Buffer::Loader::Loader(Buffer &b):
	buf(b)
{
	add("sound_data", &Loader::sound_data);
}

void Buffer::Loader::sound_data(const string &data)
{
	Sound sound;
	sound.load_memory(data.data(), data.size());

	buf.data(sound);
}

} // namespace AL
} // namespace Msp
