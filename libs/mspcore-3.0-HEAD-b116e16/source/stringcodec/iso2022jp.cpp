#include "ascii.h"
#include "iso2022jp.h"
#include "jisx0201.h"
#include "jisx0208.h"

using namespace std;

namespace Msp {
namespace StringCodec {

void Iso2022Jp::Encoder::encode_char(unichar ch, string &buf)
{
	if(ch>=0 && ch<=0x7F && ch!=0x5C && ch!=0x7E)
	{
		if(mode!=ASCII && mode!=JISX0201)
			switch_mode(ASCII, buf);
		buf += ch;
	}
	else if(ch==0x5C || ch==0x7E)
	{
		if(mode!=ASCII)
			switch_mode(ASCII, buf);
		buf += ch;
	}
	else if(ch==0xA5 || ch==0x203E)
	{
		if(mode!=JISX0201)
			switch_mode(JISX0201, buf);
		if(ch==0xA5)
			buf += 0x5C;
		else if(ch==0x203E)
			buf += 0x7E;
	}
	else
	{
		Kuten jis = ucs_to_jisx0208(ch);
		if(!jis)
			return error(ch, buf, invalid_character(ch, "ISO-2022-JP"));

		if(mode!=JISX0208)
			switch_mode(JISX0208, buf);

		char jbuf[2];
		jbuf[0] = jis.ku+0x20;
		jbuf[1] = jis.ten+0x20;
		buf.append(jbuf, 2);
	}
}

void Iso2022Jp::Encoder::sync(string &buf)
{
	if(mode!=ASCII)
		switch_mode(ASCII, buf);
}

void Iso2022Jp::Encoder::reset()
{
	mode = ASCII;
}

void Iso2022Jp::Encoder::switch_mode(Mode m, string &buf)
{
	mode = m;
	switch(mode)
	{
	case ASCII:    buf.append("\033(B", 3); break;
	case JISX0201: buf.append("\033(J", 3); break;
	case JISX0208: buf.append("\033$B", 3); break;
	default: throw invalid_argument("Iso2022Jp::Encoder::switch_mode");
	}
}

void Iso2022Jp::Encoder::transliterate(unichar, string &buf)
{
	if(mode!=ASCII)
		switch_mode(ASCII, buf);
	buf += '?';
}


Iso2022Jp::Decoder::Decoder(ErrorMode em):
	Codec::Decoder(em),
	mode(ASCII),
	dec(new Ascii::Decoder)
{ }

unichar Iso2022Jp::Decoder::decode_char(const string &str, string::const_iterator &i)
{
	if(i==str.end())
		return -1;

	while(i!=str.end())
	{
		auto j = i;

		unichar result = -1;
		if(*j==033)
		{
			unsigned escape = 0;
			for(++j; j!=str.end(); ++j)
			{
				escape = escape<<8 | static_cast<unsigned char>(*j);
				if(*j>='@' && *j<='Z')
					break;
			}

			bool ok = true;
			switch(escape)
			{
			case 0x2842: switch_mode(ASCII); break;    // ESC ( B
			case 0x284A: switch_mode(JISX0201); break; // ESC ( J
			case 0x2440:                               // ESC $ @
			case 0x2442: switch_mode(JISX0208); break; // ESC $ B
			default: ok = false;
			}

			if(ok)
				i = j;
			else
				result = *i;
			++i;
		}
		else if(dec)
			return dec->decode_char(str, i);
		else
			throw logic_error("no sub-decoder");

		if(result>=0)
			return result;
	}

	return -1;
}

void Iso2022Jp::Decoder::reset()
{
	delete dec;
	mode = ASCII;
	dec = new Ascii::Decoder;
}

void Iso2022Jp::Decoder::switch_mode(Mode m)
{
	delete dec;

	mode = m;
	switch(mode)
	{
	case ASCII: dec = new Ascii::Decoder; break;
	case JISX0201: dec = new JisX0201::Decoder; break;
	case JISX0208: dec = new JisX0208::Decoder; break;
	}
}

} // namespace StringCodec
} // namespace Msp
