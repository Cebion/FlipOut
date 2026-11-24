/* $Id: buffer.h 8 2008-08-19 09:41:12Z tdb $

This file is part of libmspal
Copyright © 2008 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_AL_BUFFER_H_
#define MSP_AL_BUFFER_H_

#include <string>
#include <AL/al.h>
#include <msp/datafile/loader.h>
#include "format.h"
#include "types.h"

namespace Msp {
namespace AL {

class Sound;

/**
Buffers are used to store audio data and feed it into a Source.
*/
class Buffer
{
public:
	class Loader: public DataFile::Loader
	{
	private:
		Buffer &buf;

	public:
		Loader(Buffer &);
	private:
		void sound_data(const std::string &);
	};

private:
	uint id;

public:
	Buffer();
	~Buffer();

	uint get_id() const { return id; }
	void data(Format, const void *, sizei, sizei);
	void data(const Sound &);
	void load_data(const std::string &);
};

} // namespace AL
} // namespace Msp

#endif
