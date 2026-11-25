/* $Id$

This file is part of libmspstrings
Copyright © 2006-2007 Mikko Rasa
Distributed under the LGPL
*/

#include "codecutils.h"
#include "iso885915.h"

using namespace std;

namespace {

const unsigned map_size=8;

const int mapping[map_size*2]=
{
	0x0152, 0xBC,
	0x0153, 0xBD,
	0x0160, 0xA6,
	0x0161, 0xA8,
	0x0178, 0xBE,
	0x017D, 0xB4,
	0x017E, 0xB8,
	0x20AC, 0xA4
};

}

namespace Msp {
namespace Codecs {

void Iso885915::Encoder::encode_char(UnicodeChar ch, string &buf)
{
	int tch=transform_mapping_or_direct(mapping, map_size, ch, false);
	if(tch<0 || tch>0xFF)
		error(ch, buf, "Can't express character in ISO-8859-15");
	else
		buf+=tch;

}

void Iso885915::Encoder::transliterate(UnicodeChar, string &buf)
{
	buf+='?';
}


UnicodeChar Iso885915::Decoder::decode_char(const string &str, string::const_iterator &i)
{
	if(i==str.end())
		return error("No input");

	unsigned char ch=*i;
	int tch=transform_mapping_or_direct(mapping, map_size, ch, true);

	UnicodeChar result;
	if(tch==-1)
		result=error("Undefined ISO-8859-15 character");
	else
		result=tch;

	++i;
	return result;
}

} // namespace Codecs
} // namespace Msp
