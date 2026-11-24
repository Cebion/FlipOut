/* $Id: textwriter.h 37 2008-02-04 15:00:02Z tdb $

This file is part of libmspdatafile
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_DATAFILE_TEXTWRITER_H_
#define MSP_DATAFILE_TEXTWRITER_H_

#include "writermode.h"

namespace Msp {
namespace DataFile {

class TextWriter: public WriterMode
{
public:
	TextWriter(IO::Base &o);
	void write(const Statement &st);
private:
	void write_(const Statement &st, unsigned);
};

} // namespace DataFile
} // namespace Msp

#endif
