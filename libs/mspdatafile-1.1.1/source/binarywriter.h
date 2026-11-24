/* $Id: binarywriter.h 43 2008-07-22 15:34:59Z tdb $

This file is part of libmspdatafile
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_DATAFILE_BINARYWRITER_H_
#define MSP_DATAFILE_BINARYWRITER_H_

#include <map>
#include "binarydict.h"
#include "writermode.h"

namespace Msp {
namespace DataFile {

/**
Writes data in binary format.
*/
class BinaryWriter: public WriterMode
{
private:
	typedef std::map<DictEntry, unsigned> Dictionary;
	typedef std::map<std::string, unsigned> StringMap;

	Dictionary dict;
	unsigned next_kwd_id;
	StringMap strings;
	unsigned next_str_id;

public:
	BinaryWriter(IO::Base &o);
	void write(const Statement &st);
private:
	void write_(const Statement &st);
	DictEntry create_entry(const Statement &st);
	void collect_keywords(const Statement &st);
	void write_int(long long n);
	void write_string(const std::string &s);
	void write_float(float f);
	void write_enum(const std::string &e);
};

} // namespace DataFile
} // namespace Msp

#endif
