/* $Id: input.h 44 2008-07-23 13:02:32Z tdb $

This file is part of libmspdatafile
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#ifndef MSP_DATAFILE_INPUT_H_
#define MSP_DATAFILE_INPUT_H_

#include <msp/io/base.h>

namespace Msp {
namespace DataFile {

class Input
{   
public:
	Input(IO::Base &); 
	int get();
	int peek();
	unsigned get_line_number() const { return line; }
	operator bool() const;
private:
	IO::Base &in;
	unsigned line;
	int next;
};  

} // namespace DataFile
} // namespace Msp

#endif
