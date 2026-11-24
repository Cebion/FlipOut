/* $Id: writermode.h 37 2008-02-04 15:00:02Z tdb $

This file is part of libmspdatafile
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_DATAFILE_WRITERMODE_H_
#define MSP_DATAFILE_WRITERMODE_H_

#include <msp/io/base.h>

namespace Msp {
namespace DataFile {

class Statement;

class WriterMode
{
protected:
	IO::Base &out;

	WriterMode(IO::Base &o): out(o) { }
public:
	virtual ~WriterMode() { }

	virtual void write(const Statement &st) =0;
};

} // namespace DataFile
} // namespace Msp

#endif
