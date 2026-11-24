#include "utf16.h"

using namespace std;

namespace Msp {
namespace StringCodec {

Utf16::Encoder::Encoder(ErrorMode em, Endian en):
	Codec::Encoder(em),
	endian(en==AUTO ? BIG : en),
	emit_bom(true)
{ }

void Utf16::Encoder::encode_char(unichar ch, string &buf)
{
	if(!is_valid_unichar(ch))
		return error(ch, buf, invalid_character(ch, "UTF-16"));

	if(emit_bom)
	{
		if(endian==LITTLE)
			buf.append("\xFF\xFE");
		else
			buf.append("\xFE\xFF");
		emit_bom = false;
	}

	bool e = (endian==LITTLE);
	if(ch<0x10000)
	{
		char utf[2];
		utf[e] = ch>>8;
		utf[1-e] = ch;
		buf.append(utf, 2);
	}
	else
	{
		char utf[4];
		ch -= 0x10000;
		unichar sur = 0xD800+((ch>>10)&0x3FF);
		utf[e] = sur>>8;
		utf[1-e] = sur;
		sur = 0xDC00+(ch&0x3FF);
		utf[2+e] = sur>>8;
		utf[3-e] = sur;
		buf.append(utf, 4);
	}
}

void Utf16::Encoder::transliterate(unichar, string &buf)
{
	if(endian==LITTLE)
		buf.append("\xFD\xFF", 2);
	else
		buf.append("\xFF\xFD", 2);
}


Utf16::Decoder::Decoder(ErrorMode em, Endian en):
	Codec::Decoder(em),
	endian(en)
{ }

unichar Utf16::Decoder::decode_char(const string &str, string::const_iterator &i)
{
	if(i==str.end())
		return -1;

	auto j = i;

	unichar unit = decode_unit(str, i, j);
	if(unit!=-1)
	{
		if(endian==AUTO)
		{
			/* Set endian based on the first decoded unit.  If the unit was a BOM,
			discard it. */
			if(unit==0xFFFE)
			{
				endian = LITTLE;
				unit = -1;
			}
			else
			{
				endian = BIG;
				if(unit==0xFEFF)
					unit = -1;
			}
		}

		if(unit==-1 && j!=str.end())
			unit = decode_unit(str, i, j);
	}

	unichar result = -1;
	if(unit!=-1)
	{
		if(unit>=0xD800 && unit<=0xDBFF)
		{
			auto k = j;

			unichar unit2 = -2;
			if(k!=str.end())
				unit2 = decode_unit(str, i, k);

			if(unit2>=0xDC00 && unit2<=0xDFFF)
			{
				j = k;
				result = 0x10000 + ((unit&0x3FF)<<10) + (unit2&0x3FF);
			}
			else if(unit2!=-1)
				result = error(invalid_sequence(i, j, "incomplete UTF-16 surrogate pair"));
		}
		else if(unit>=0xDC00 && unit<=0xDFFF)
			result = error(invalid_sequence(i, j, "stray UTF-16 trail surrogate"));
		else
			result = unit;
	}

	i = j;
	return result;
}

unichar Utf16::Decoder::decode_unit(const string &str, const string::const_iterator &i, string::const_iterator &j)
{
	unsigned char b1 = *j++;
	if(j==str.end())
		return error(invalid_sequence(i, j, "incomplete UTF-16 character"));
	unsigned char b2 = *j++;

	if(endian==LITTLE)
		return (b2<<8) | b1;
	else
		return (b1<<8) | b2;
}

} // namespace StringCodec
} // namespace Msp
