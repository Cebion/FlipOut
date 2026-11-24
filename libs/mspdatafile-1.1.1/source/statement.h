/* $Id: statement.h 38 2008-03-09 01:02:43Z tdb $

This file is part of libmspdatafile
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#ifndef MSP_DATAFILE_STATEMENT_H_
#define MSP_DATAFILE_STATEMENT_H_

#include <list>
#include "value.h"

namespace Msp {
namespace DataFile {

class Statement
{
public:
	std::string keyword;
	ValueArray  args;
	bool        valid;
	std::string source;
	unsigned    line;
	std::list<Statement> sub;

	Statement(): valid(false), line(0) { }
	Statement(const std::string &kw): keyword(kw), valid(true), line(0) { }
	std::string get_location() const;
	
	template<typename T>
	Statement &append(const T &v)
	{
		args.push_back(Value(v));
		return *this;
	}

	template<typename T>
	Statement &operator,(const T &v)
	{ return append(v); }
};

} // namespace DataFile
} // namespace Msp

#endif
