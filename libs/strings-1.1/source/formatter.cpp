/* $Id$

This file is part of libmspstrings
Copyright © 2006-2007 Mikko Rasa
Distributed under the LGPL
*/

#include "formatter.h"

using namespace std;

namespace Msp {

Formatter::Formatter(const string &f):
	fmt(f),
	pos(fmt.begin())
{
	advance();
}

/**
Returns the result of the formatting operation.  Will throw if not enough
values have been fed to the formatter.
*/
const string &Formatter::str() const
{
	if(pos!=fmt.end())
		throw Exception("Too few arguments for format");

	return result;
}

/**
Advances the pos iterator to the next conversion, adding literal characters to
the result.  The iterator is left at the second character of the conversion
(i.e. after the %).
*/
void Formatter::advance()
{
	for(; pos!=fmt.end(); ++pos)
	{
		if(*pos=='%')
		{
			++pos;
			if(pos==fmt.end())
				throw Exception("Malformed format string");
			if(*pos!='%')
				break;
		}

		result+=*pos;
	}
}

/**
Reads the next conversion from the format string and returns a corresponding
Fmt object.
*/
Fmt Formatter::get_conversion()
{
	if(pos==fmt.end())
		throw Exception("Too many arguments for format");

	string::iterator i=pos;
	for(; i!=fmt.end(); ++i)
		if(isalpha(*i))
			break;

	if(i==fmt.end())
		throw Exception("Malformed format string");

	++i;
	string c(pos, i);
	pos=i;

	return Fmt(c);
}

} // namespace Msp
