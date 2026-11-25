/* $Id$

This file is part of libmspstrings
Copyright © 2006-2007 Mikko Rasa
Distributed under the LGPL
*/

#include "utf8.h"

using namespace std;

namespace Msp {
namespace Codecs {

void Utf8::Encoder::encode_char(UnicodeChar ch, string &buf)
{
	if(ch<0 || ch>0x10FFFF)
		return error(ch, buf, "Can't express character in UTF-8");

	unsigned bytes=1;
	if(ch>0xFFFF)
		bytes=4;
	else if(ch>0x7FF)
		bytes=3;
	else if(ch>0x7F)
		bytes=2;

	if(bytes==1)
		buf+=ch;
	else
	{
		char utf[4];

		utf[0]=0xFF<<(8-bytes) | ch>>(bytes*6-6);
		for(unsigned j=bytes-1; j>0; --j)
		{
			utf[j]=0x80 | (ch&0x3F);
			ch>>=6;
		}

		buf.append(utf, bytes);
	}
}

void Utf8::Encoder::transliterate(UnicodeChar, string &buf)
{
	buf.append("\357\277\275", 3);  // � U+FFFE Replacement Character
}


UnicodeChar Utf8::Decoder::decode_char(const string &str, string::const_iterator &i)
{
	if(i==str.end())
		return error("No input");

	if((*i&0xC0)==0x80)
	{
		UnicodeChar result=error("UTF-8 tail byte found when expecting head");
		++i;
		return result;
	}
	else if(*i&0x80)
	{
		unsigned bytes=2;
		unsigned mask=0x20;
		for(; *i&mask; mask>>=1)
			++bytes;

		string::const_iterator j=i;

		UnicodeChar result=(*j++)&(mask-1);

		unsigned k;
		for(k=1; (k<bytes && j!=str.end() && (*j&0xC0)==0x80); ++k)
			result=(result<<6) | ((*j++)&0x3F);

		if(k<bytes)
			result=error("Incomplete UTF-8 character");
		else if(!(result>>(bytes*5-4)) || !(result>>7))
			result=error("Denormalized UTF-8 multibyte sequence");
		else if(result>0x10FFFF || (result>=0xD800 && result<=0xDFFF))
			result=error("Invalid Unicode code point");

		i=j;
		return result;
	}
	else
		return *i++;
}

} // namespace Codecs
} // namespace Msp
