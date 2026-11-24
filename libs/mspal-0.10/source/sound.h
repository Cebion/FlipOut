/* $Id: sound.h 8 2008-08-19 09:41:12Z tdb $

This file is part of libmspal
Copyright © 2008 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_AL_SOUND_H_
#define MSP_AL_SOUND_H_

#include <string>
#include <vorbis/vorbisfile.h>
#include "format.h"

namespace Msp {
namespace AL {

/**
This class facilitates loading sound files.  Currently only Ogg Vorbis is
supported.
*/
class Sound
{
private:
	OggVorbis_File ovfile;
	unsigned freq;
	unsigned size;
	char *data;
	Format format;
	unsigned read_pos;
	bool eof_flag;

public:
	Sound();
	~Sound();

	void open_file(const std::string &);
	void open_memory(const void *, unsigned);
	void load_data();
	void load_file(const std::string &);
	void load_memory(const void *, unsigned);
	void close();
	void rewind();
	unsigned read(char *, unsigned);
	bool eof() const { return eof_flag; }

	Format get_format() const { return format; }
	unsigned get_frequency() const { return freq; }
	unsigned get_size() const { return size; }
	const char *get_data() const;
private:
	void open_common();
};

} // namespace AL
} // namespace Msp

#endif
