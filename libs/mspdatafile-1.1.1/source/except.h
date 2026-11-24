/* $Id: except.h 45 2008-08-06 16:35:21Z tdb $

This file is part of libmspdatafile
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_DATAFILE_EXCEPT_H_
#define MSP_DATAFILE_EXCEPT_H_

#include <msp/core/except.h>

namespace Msp {
namespace DataFile {

class TypeError: public Exception
{
public:
	TypeError(const std::string &w): Exception(w) { }
};

class ParseError: public Exception
{
public:
	ParseError(const std::string &w): Exception(w) { }
};

} // namespace DataFile
} // namespace Msp

#endif
