/* $Id: writer.h 41 2008-07-08 15:23:07Z tdb $

This file is part of libmspdatafile
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_DATAFILE_WRITER_H_
#define MSP_DATAFILE_WRITER_H_

#include <map>
#include <msp/io/base.h>
#include "binarydict.h"

namespace Msp {
namespace DataFile {

class Statement;
class WriterMode;

/**
Frontend for writing data.
*/
class Writer
{
private:
	IO::Base &out;
	WriterMode *mode;
	bool binary;

public:
	Writer(IO::Base &o);
	~Writer();

	/**
	Writes a statement to the output.  This function always writes a complete
	statement, so it's not possible to add substatements later.
	*/
	void write(const Statement &st);

	/**
	Sets binary or text mode.  While it is possible to enter and exit binary
	mode multiple times, doing so produces sub-optimal output.

	@param  b  true for binary mode, false for text
	*/
	void set_binary(bool b);
};

} // namespace DataFile
} // namespace Msp

#endif
