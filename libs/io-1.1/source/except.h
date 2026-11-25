/* $Id$

This file is part of libmspio
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#ifndef MSP_IO_EXCEPT_H_
#define MSP_IO_EXCEPT_H_

#include <msp/core/except.h>

namespace Msp {
namespace IO {

class FileNotFound: public Exception
{
public:
	FileNotFound(const std::string &w_, const std::string &f): Exception(w_), filename(f) { }
	const std::string &get_filename() { return filename; }
	~FileNotFound() throw() { }
private:
	std::string filename;
};

} // namespace IO
} // namespace Msp

#endif
