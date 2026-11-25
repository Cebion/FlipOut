/* $Id$

This file is part of libmspstrings
Copyright © 2006-2007 Mikko Rasa
Distributed under the LGPL
*/

#include "windows1252.h"

using namespace std;

namespace {

unsigned short table[32]=
{
	0x20AC, 0,      0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021,
	0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0,      0x017D, 0,
	0,      0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
	0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0,      0x017E, 0x0178
};

}

namespace Msp {
namespace Codecs {

void Windows1252::Encoder::encode_char(UnicodeChar ch, string &buf)
{
	if((ch>=0 && ch<=0x7F) || (ch>=0xA0 && ch<=0xFF))
		buf+=ch;
	else
	{
		for(unsigned i=0; i<32; ++i)
			if(table[i]==ch)
			{
				buf+=ch;
				return;
			}

		error(ch, buf, "Can't express character in Windows-1252");
	}
}

void Windows1252::Encoder::transliterate(UnicodeChar, string &buf)
{
	buf+='?';
}


UnicodeChar Windows1252::Decoder::decode_char(const string &str, string::const_iterator &i)
{
	if(i==str.end())
		return error("No input");

	int ch=static_cast<unsigned char>(*i);
	UnicodeChar result;
	if(ch>=0x80 && ch<=0x9F)
	{
		result=table[ch-0x80];
		if(result==0)
			result=error("Undefined Windows-1252 character");
	}
	else
		result=ch;

	++i;
	return result;
}

} // namespace Codecs
} // namespace Msp
