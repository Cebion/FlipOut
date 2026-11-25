/* $Id$

This file is part of libmspstrings
Copyright © 2006-2007 Mikko Rasa
Distributed under the LGPL
*/

#include "codecutils.h"
#include "iso646fi.h"

using namespace std;

namespace {

const unsigned map_size=9;

const int mapping[map_size*2]=
{
	0xC4, 0x5B,
	0xC5, 0x5D,
	0xD6, 0x5C,
	0xDC, 0x5E,
	0xE4, 0x7B,
	0xE5, 0x7D,
	0xE9, 0x60,
	0xF6, 0x7C,
	0xFC, 0x7E
};

} // namespace

namespace Msp {
namespace Codecs {

void Iso646Fi::Encoder::encode_char(UnicodeChar ch, string &buf)
{
	int tch=transform_mapping_or_direct(mapping, map_size, ch, false);
	if(tch<0 || tch>0x7F)
		error(ch, buf, "Can't express character in ISO-646-FI");
	else
		buf+=tch;
}

void Iso646Fi::Encoder::transliterate(UnicodeChar, string &buf)
{
	buf+='?';
}


UnicodeChar Iso646Fi::Decoder::decode_char(const string &str, string::const_iterator &i)
{
	if(i==str.end())
		return error("No input");

	unsigned char ch=*i;
	int tch=(ch<=0x7F ? transform_mapping_or_direct(mapping, map_size, ch, true) : -1);

	UnicodeChar result;
	if(tch==-1)
		result=error("Undefined ISO-646-FI character");
	else
		result=tch;

	++i;
	return result;
}

} // namespace Codecs
} // namespace Msp
