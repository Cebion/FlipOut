#include "utf8.h"

using namespace std;

namespace Msp {
namespace StringCodec {

void Utf8::Encoder::encode_char(unichar ch, string &buf)
{
	if(!is_valid_unichar(ch))
		return error(ch, buf, invalid_character(ch, "UTF-8"));

	unsigned bytes = 1;
	if(ch>0xFFFF)
		bytes = 4;
	else if(ch>0x7FF)
		bytes = 3;
	else if(ch>0x7F)
		bytes = 2;

	if(bytes==1)
		buf += ch;
	else
	{
		char utf[4];

		utf[0] = 0xFF<<(8-bytes) | ch>>(bytes*6-6);
		for(unsigned j=bytes-1; j>0; --j)
		{
			utf[j] = 0x80 | (ch&0x3F);
			ch >>= 6;
		}

		buf.append(utf, bytes);
	}
}

void Utf8::Encoder::transliterate(unichar, string &buf)
{
	buf.append("\357\277\275", 3);  // � U+FFFD Replacement Character
}


unichar Utf8::Decoder::decode_char(const string &str, string::const_iterator &i)
{
	if(i==str.end())
		return -1;

	if((*i&0xC0)==0x80)
	{
		unichar result = error(invalid_sequence(i, i+1, "stray UTF-8 head byte"));
		++i;
		return result;
	}
	else if(*i&0x80)
	{
		unsigned bytes = 2;
		unsigned mask = 0x20;
		for(; *i&mask; mask>>=1)
			++bytes;

		auto j = i;

		unichar result = (*j++)&(mask-1);

		unsigned k;
		for(k=1; (k<bytes && j!=str.end() && (*j&0xC0)==0x80); ++k)
			result = (result<<6) | ((*j++)&0x3F);

		if(k<bytes)
			result = error(invalid_sequence(i, j, "incomplete UTF-8 character"));
		else if(!(result>>(bytes*5-4)) || !(result>>7))
			result = error(invalid_sequence(i, j, "denormalized UTF-8 sequence"));
		else if(!is_valid_unichar(result))
			result = error(invalid_sequence(i, j, "undefined UTF-8 character"));

		i = j;
		return result;
	}
	else
		return *i++;
}

} // namespace StringCodec
} // namespace Msp
