/* $Id: parser.h 53 2008-09-13 18:03:04Z tdb $

This file is part of libmspdatafile
Copyright © 2006-2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_DATAFILE_PARSER_H_
#define MSP_DATAFILE_PARSER_H_

#include <string>
#include "input.h"

namespace Msp {
namespace DataFile {

class ParserMode;
class Statement;
struct Token;

/**
Frontend for loading datafiles.  Handles switching between text and binary
formats.  A Parser evaluates into a boolean value indicating whether more
statements may be read.
*/
class Parser
{
private:
	Input       in;
	std::string main_src;
	std::string src;
	bool        good;
	ParserMode  *mode;

public:
	Parser(IO::Base &i, const std::string &s);
	~Parser();

	/**
	Reads a statement from the input.  If the end of input was reached, an empty
	invalid statement will be returned.  If an error occurs, the parser will be
	marked as bad and no more statements may be read, even if the exception was
	caught.
	*/
	Statement parse();

	operator bool() const { return good && in; }
};

} // namespace DataFile
} // namespace Msp

#endif
