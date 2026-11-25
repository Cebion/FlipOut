/* $Id$

This file is part of libmspstrings
Copyright © 2006-2007 Mikko Rasa
Distributed under the LGPL
*/

#include "iso88591.h"

using namespace std;

namespace Msp {
namespace Codecs {

void Iso88591::Encoder::encode_char(UnicodeChar ch, string &buf)
{
	if(ch<0 || ch>0xFF)
		return error(ch, buf, "Can't express character in ISO-8859-1");

	buf+=ch;
}

void Iso88591::Encoder::transliterate(UnicodeChar, string &buf)
{
	buf+='?';
}


UnicodeChar Iso88591::Decoder::decode_char(const string &str, string::const_iterator &i)
{
	if(i==str.end())
		return error("No input");

	return static_cast<unsigned char>(*i++);
}

} // namespace Codecs
} // namespace Msp
