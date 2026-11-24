#include "ascii.h"
#include "codec.h"
#include "iso2022jp.h"
#include "iso646fi.h"
#include "iso88591.h"
#include "iso885915.h"
#include "jisx0201.h"
#include "jisx0208.h"
#include "utf16.h"
#include "utf8.h"
#include "windows1252.h"

using namespace std;

namespace Msp {
namespace StringCodec {

bool Codec::detect(const string &str) const
{
	Decoder *dec = create_decoder(IGNORE_ERRORS);

	bool result = true;
	for(auto i=str.begin(); (result && i!=str.end()); )
		result = (dec->decode_char(str, i)!=-1);

	delete dec;

	return result;
}

void Codec::Encoder::encode(const ustring &str, string &buf)
{
	for(unichar c: str)
		encode_char(c, buf);
}

string Codec::Encoder::encode(const ustring &str)
{
	string buf;
	encode(str, buf);
	sync(buf);
	return buf;
}



void Codec::Decoder::decode(const string &str, ustring &buf)
{
	for(auto i=str.begin(); i!=str.end();)
	{
		unichar c = decode_char(str, i);
		if(c!=-1)
			buf += c;
	}
}

ustring Codec::Decoder::decode(const string &str)
{
	ustring buf;
	decode(str, buf);
	return buf;
}

Codec *create_codec(const string &n)
{
	string name;
	string::const_iterator i;
	for(i=n.begin(); i!=n.end(); ++i)
	{
		if(*i==':')
			break;
		else if(isupper(*i))
			name += tolower(*i);
		else if(islower(*i) || isdigit(*i))
			name += *i;
	}

	ErrorMode em = DEFAULT;
	if(i!=n.end() && *i==':')
	{
		string em_str(i+1, n.end());
		if(em_str=="throw")
			em = THROW_ON_ERROR;
		else if(em_str=="ignore")
			em = IGNORE_ERRORS;
		else if(em_str=="trans" || em_str=="transliterate")
			em = TRANSLITERATE;
		else
			throw invalid_argument("invalid error mode");
	}

	if(name=="ascii") return new Ascii(em);
	if(name=="iso2022jp") return new Iso2022Jp(em);
	if(name=="iso646fi") return new Iso646Fi(em);
	if(name=="iso88591" || name=="latin1") return new Iso88591(em);
	if(name=="iso885915" || name=="latin9") return new Iso885915(em);
	if(name=="jisx0201") return new JisX0201(em);
	if(name=="jisx0208") return new JisX0208(em);
	if(name=="utf8") return new Utf8(em);
	if(name=="utf16") return new Utf16(em, Utf16::AUTO);
	if(name=="utf16be") return new Utf16(em, Utf16::BIG);
	if(name=="utf16le") return new Utf16(em, Utf16::LITTLE);
	if(name=="windows1252" || name=="cp1252") return new Windows1252(em);
	throw invalid_argument("unknown string codec");
}

Codec *detect_codec(const string &str)
{
	bool is_utf8 = true;
	bool is_ascii = true;
	bool is_latin1 = true;
	unsigned utf8_mb = 0;

	for(char c: str)
	{
		if(c&0x80)
		{
			is_ascii = false;
			if((c&0xC0)==0x80)
			{
				if((c&0xE0)==0x80)
					is_latin1 = false;
				if(utf8_mb)
					--utf8_mb;
				else
					is_utf8 = false;
			}
			else if((c&0xC0)==0xC0)
			{
				if(utf8_mb)
				{
					is_utf8 = false;
					utf8_mb = 0;
				}
				else
				{
					for(utf8_mb=1; (c>>(6-utf8_mb))&1; ++utf8_mb) ;
				}
			}
		}
		else if(utf8_mb)
		{
			is_utf8 = false;
			utf8_mb = 0;
		}
	}

	if(is_ascii)
		return new Ascii;
	else if(is_utf8)
		return new Utf8;
	else if(is_latin1)
		return new Iso88591;
	else
		return new Windows1252;
}

} // namespace StringCodec
} // namespace Msp
