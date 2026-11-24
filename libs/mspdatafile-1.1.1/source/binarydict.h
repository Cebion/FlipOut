/* $Id: binarydict.h 25 2007-09-17 17:09:56Z tdb $

This file is part of libmspdatafile
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_DATAFILE_BINARYDICT_H_
#define MSP_DATAFILE_BINARYDICT_H_

#include <string>

namespace Msp {
namespace DataFile {

/**
Stores statement information for binary files.
*/
struct DictEntry
{
	std::string keyword;
	std::string args;

	DictEntry() { }
	DictEntry(const std::string &k, const std::string &a): keyword(k), args(a) { }

	bool operator<(const DictEntry &o) const
	{ return keyword<o.keyword || (keyword==o.keyword && args<o.args); }
};

} // namespace DataFile
} // namespace Msp

#endif
