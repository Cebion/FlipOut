#include "codecutils.h"
#include "iso646fi.h"

using namespace std;

namespace {

const unsigned map_size = 9;

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

}


namespace Msp {
namespace StringCodec {

void Iso646Fi::Encoder::encode_char(unichar ch, string &buf)
{
	int tch = transform_mapping_or_direct(mapping, map_size, ch, false);
	if(tch<0 || tch>0x7F)
		error(ch, buf, invalid_character(ch, "ISO-646-FI"));
	else
		buf += tch;
}

void Iso646Fi::Encoder::transliterate(unichar, string &buf)
{
	buf += '?';
}


unichar Iso646Fi::Decoder::decode_char(const string &str, string::const_iterator &i)
{
	if(i==str.end())
		return -1;

	unsigned char ch = *i;
	unichar result;
	if(ch>=0x80)
		result = error(invalid_sequence(i, i+1, "undefined ISO-646-FI character"));
	else
		result = transform_mapping_or_direct(mapping, map_size, ch, true);

	++i;
	return result;
}

} // namespace StringCodec
} // namespace Msp
