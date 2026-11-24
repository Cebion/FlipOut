#include "iso88591.h"

using namespace std;

namespace Msp {
namespace StringCodec {

void Iso88591::Encoder::encode_char(unichar ch, string &buf)
{
	if(ch<0 || ch>0xFF)
		return error(ch, buf, invalid_character(ch, "ISO-8859-1"));

	buf += ch;
}

void Iso88591::Encoder::transliterate(unichar, string &buf)
{
	buf += '?';
}


unichar Iso88591::Decoder::decode_char(const string &str, string::const_iterator &i)
{
	if(i==str.end())
		return -1;

	return static_cast<unsigned char>(*i++);
}

} // namespace StringCodec
} // namespace Msp
