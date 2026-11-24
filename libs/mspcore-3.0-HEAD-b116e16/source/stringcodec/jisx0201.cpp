#include "jisx0201.h"

using namespace std;

namespace Msp {
namespace StringCodec {

void JisX0201::Encoder::encode_char(unichar ch, string &buf)
{
	if(ch>=0 && ch<=0x7F && ch!=0x5C && ch!=0x7E)
		buf += ch;
	else if(ch==0xA5)
		buf += 0x5C;
	else if(ch==0x203E)
		buf += 0x7E;
	else if(ch>=0xFF61 && ch<=0xFF9F)
		buf += ch-0xFEC0;
	else
		error(ch, buf, invalid_character(ch, "JIS X 0201"));
}

void JisX0201::Encoder::transliterate(unichar, string &buf)
{
	buf += '?';
}


unichar JisX0201::Decoder::decode_char(const string &str, string::const_iterator &i)
{
	if(i==str.end())
		return -1;

	unsigned char ch = *i;
	unichar result;
	if(ch==0x5C)
		result = 0xA5;
	else if(ch==0x7E)
		result = 0x203E;
	else if(ch<=0x7F)
		result = ch;
	else if(ch>=0xA1 && ch<=0xDF)
		result = ch+0xFEC0;
	else
		result = error(invalid_sequence(i, i+1, "undefined JIS X 0201 character"));

	++i;
	return result;
}

} // namespace StringCodec
} // namespace Msp
