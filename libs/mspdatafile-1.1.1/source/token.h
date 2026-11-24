/* $Id: token.h 19 2007-08-21 14:11:23Z tdb $

This file is part of libmspdatafile
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#ifndef MSP_DATAFILE_TOKEN_H_
#define MSP_DATAFILE_TOKEN_H_

namespace Msp {
namespace DataFile {

struct Token
{
	enum Type
	{
		INTEGER,
		FLOAT,
		STRING,
		IDENTIFIER,
		SPECIAL
	};

	Type type;
	std::string str;

	Token(): type(SPECIAL) { }
	Token(Type t, const std::string &s): type(t), str(s) { }
};

} // namespace DataFile
} // namespace Msp

#endif
