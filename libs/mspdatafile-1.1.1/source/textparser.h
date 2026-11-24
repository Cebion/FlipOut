/* $Id: textparser.h 45 2008-08-06 16:35:21Z tdb $

This file is part of libmspdatafile
Copyright © 2007-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_DATAFILE_TEXTPARSER_H_
#define MSP_DATAFILE_TEXTPARSER_H_

#include "parsermode.h"

namespace Msp {
namespace DataFile {

class Token;

class TextParser: public ParserMode
{
public:
	TextParser(Input &, const std::string &);
	virtual Statement parse();
protected:
	Statement parse_statement(const Token *);
	Token parse_token();
	bool  is_delimiter(int);
	bool  isodigit(int);
	std::string get_location();
	void  parse_error(int, const char *);
};

} // namespace DataFile
} // namespace Msp

#endif
