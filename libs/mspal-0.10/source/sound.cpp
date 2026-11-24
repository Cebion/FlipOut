/* $Id: sound.cpp 10 2009-07-26 17:55:52Z tdb $

This file is part of libmspal
Copyright © 2008-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cstring>
#include <msp/core/except.h>
#include "sound.h"

using namespace std;

namespace {

struct MemorySource
{
	const void *data;
	unsigned length;
	unsigned pos;

	MemorySource(const void *d, unsigned l): data(d), length(l), pos(0) { }
};

size_t memory_read(void *ptr, size_t size, size_t nmemb, void *src)
{
	MemorySource &memsrc=*reinterpret_cast<MemorySource *>(src);
	unsigned len=min(size*nmemb, memsrc.length-memsrc.pos);
	memcpy(ptr, reinterpret_cast<const char *>(memsrc.data)+memsrc.pos, len);
	memsrc.pos+=len;

	return len/size;
}

int memory_seek(void *src, ogg_int64_t offset, int whence)
{
	MemorySource &memsrc=*reinterpret_cast<MemorySource *>(src);
	if(whence==SEEK_SET)
		memsrc.pos=offset;
	else if(whence==SEEK_CUR)
		memsrc.pos+=offset;
	else if(whence==SEEK_END)
		memsrc.pos=memsrc.length-offset;
	memsrc.pos=min(memsrc.pos, memsrc.length);

	return memsrc.pos;
}

int memory_close(void *src)
{
	delete reinterpret_cast<MemorySource *>(src);
	return 0;
}

long memory_tell(void *src)
{
	MemorySource &memsrc=*reinterpret_cast<MemorySource *>(src);
	return memsrc.pos;
}

ov_callbacks memory_callbacks=
{
	&memory_read,
	&memory_seek,
	&memory_close,
	&memory_tell
};

} // namespace

namespace Msp {
namespace AL {

Sound::Sound():
	data(0),
	eof_flag(false)
{
	ovfile.datasource=0;
}

Sound::~Sound()
{
	delete[] data;
	if(ovfile.datasource)
		ov_clear(&ovfile);
}

void Sound::open_file(const string &fn)
{
	if(ovfile.datasource)
		throw InvalidState("Sound has already been opened");
	if(ov_fopen(const_cast<char *>(fn.c_str()), &ovfile)<0)
		throw Exception("Could not open ogg vorbis file "+fn);

	open_common();
}

void Sound::open_memory(const void *d, unsigned len)
{
	if(ovfile.datasource)
		throw InvalidState("Sound has already been opened");

	MemorySource *src=new MemorySource(d, len);
	if(ov_open_callbacks(src, &ovfile, 0, 0, memory_callbacks)<0)
	{
		delete src;
		throw Exception("Could not open ogg vorbis memory block");
	}

	open_common();
}

void Sound::load_data()
{
	if(data)
		throw InvalidState("Data has already been loaded");

	size=ov_pcm_total(&ovfile, 0)*4;
	char *dptr=new char[size];
	unsigned pos=0;
	while(unsigned len=read(dptr+pos, size-pos))
		pos+=len;
	data=dptr;
	size=pos;
	read_pos=0;
}

void Sound::load_file(const std::string &fn)
{
	open_file(fn);
	load_data();
	close();
}

void Sound::load_memory(const void *d, unsigned len)
{
	open_memory(d, len);
	load_data();
	close();
}

void Sound::close()
{
	if(ovfile.datasource)
		ov_clear(&ovfile);
}

void Sound::rewind()
{
	if(data)
		read_pos=0;
	else
		ov_pcm_seek(&ovfile, 0);
}

unsigned Sound::read(char *buf, unsigned len)
{
	if(data)
	{
		len=min(len, size-read_pos);
		memcpy(buf, data+read_pos, len);
		read_pos+=len;
		return len;
	}
	else if(ovfile.datasource)
	{
		int section=0;
		int res=ov_read(&ovfile, buf, len, 0, 2, 1, &section);
		if(res<0)
			throw Exception("Error reading ogg vorbis file");
		else if(res==0)
			eof_flag=true;
		return res;
	}
	else
		throw InvalidState("No data available");
}

const char *Sound::get_data() const
{
	if(!data)
		throw InvalidState("Data has not been loaded");
	return data;
}

void Sound::open_common()
{
	delete data;
	data=0;

	vorbis_info *info=ov_info(&ovfile, -1);
	freq=info->rate;
	switch(info->channels)
	{
	case 1: format=MONO16; break;
	case 2: format=STEREO16; break;
	default: throw Exception("Unsupported number of channels");
	}
}

} // namespace AL
} // namespace Msp
