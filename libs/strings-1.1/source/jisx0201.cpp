/* $Id$

This file is part of libmspstrings
Copyright © 2006-2007 Mikko Rasa
Distributed under the LGPL
*/

#include "jisx0201.h"

using namespace std;

namespace Msp {
namespace Codecs {

void JisX0201::Encoder::encode_char(UnicodeChar ch, string &buf)
{
	if(ch>=0 && ch<=0x7F && ch!=0x5C && ch!=0x7E)
		buf+=ch;
	else if(ch==0xA5)
		buf+=0x5C;
	else if(ch==0x203E)
		buf+=0x7E;
	else if(ch>=0xFF61 && ch<=0xFF9F)
		buf+=ch-0xFEC0;
	else
		error(ch, buf, "Can't express character in JIS X 0201");
}

void JisX0201::Encoder::transliterate(UnicodeChar, string &buf)
{
	buf+='?';
}


UnicodeChar JisX0201::Decoder::decode_char(const string &str, string::const_iterator &i)
{
	if(i==str.end())
		return error("No input");

	unsigned char ch=*i;
	UnicodeChar result;
	if(ch==0x5C)
		result=0xA5;
	else if(ch==0x7E)
		result=0x203E;
	else if(ch<=0x7F)
		result=ch;
	else if(ch>=0xA1 && ch<=0xDF)
		result=ch+0xFEC0;
	else
		result=error("Undefined JIS X 0201 character");

	++i;
	return result;
}

} // namespace Codecs
} // namespace Msp
