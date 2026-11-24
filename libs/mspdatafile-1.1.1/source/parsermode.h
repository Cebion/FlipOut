/* $Id: parsermode.h 53 2008-09-13 18:03:04Z tdb $

This file is part of libmspdatafile
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_DATAFILE_PARSERMODE_H_
#define MSP_DATAFILE_PARSERMODE_H_

#include "statement.h"

namespace Msp {
namespace DataFile {

class Input;

/**
Base class for parse modes.
*/
class ParserMode
{
protected:
	Input &in;
	const std::string &src;

	ParserMode(Input &i, const std::string &s): in(i), src(s) { }
public:
	virtual ~ParserMode() { }

	virtual Statement parse() =0;
};

} // namespace DataFile
} // namespace Msp

#endif
