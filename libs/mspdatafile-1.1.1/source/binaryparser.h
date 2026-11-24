/* $Id: binaryparser.h 43 2008-07-22 15:34:59Z tdb $

This file is part of libmspdatafile
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_DATAFILE_BINARYPARSER_H_
#define MSP_DATAFILE_BINARYPARSER_H_

#include <map>
#include "binarydict.h"
#include "parsermode.h"

namespace Msp {
namespace DataFile {

/**
Parses data in binary format.
*/
class BinaryParser: public ParserMode
{
private:
	typedef std::map<unsigned, DictEntry> Dictionary;
	typedef std::map<unsigned, std::string> StringMap;

	Dictionary dict;
	StringMap strings;
	bool first;

public:
	BinaryParser(Input &i, const std::string &s);
	Statement parse();
private:
	Statement parse_statement();
	long long parse_int();
	float parse_float();
	std::string parse_string();
	bool parse_bool();
	std::string parse_enum();
	const std::string &lookup_string(unsigned) const;
};

} // namespace DataFile
} // namespace Msp

#endif
