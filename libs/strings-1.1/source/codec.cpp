/* $Id$

This file is part of libmspstrings
Copyright © 2006-2007 Mikko Rasa
Distributed under the LGPL
*/

#include "ascii.h"
#include "codec.h"
#include "iso2022jp.h"
#include "iso646fi.h"
#include "iso88591.h"
#include "iso885915.h"
#include "jisx0201.h"
#include "jisx0208.h"
#include "utf8.h"
#include "windows1252.h"

using namespace std;

namespace Msp {
namespace Codecs {

bool Codec::detect(const string &str) const
{
	Decoder *dec=create_decoder();
	bool result=true;
	try
	{
		for(string::const_iterator i=str.begin(); i!=str.end(); )
			dec->decode_char(str, i);
	}
	catch(const CodecError &)
	{
		result=false;
	}

	delete dec;

	return result;
}

void Codec::Encoder::encode(const ustring &str, string &buf)
{
	for(ustring::const_iterator i=str.begin(); i!=str.end(); ++i)
		encode_char(*i, buf);
}

string Codec::Encoder::encode(const ustring &str)
{
	string buf;
	encode(str, buf);
	sync(buf);
	return buf;
}

void Codec::Encoder::error(UnicodeChar ch, string &buf, const string &msg)
{
	switch(err_mode)
	{
	case TRANSLITERATE:
		transliterate(ch, buf);
	case IGNORE_ERRORS:
		break;
	default:
		throw CodecError(msg);
	}
}


void Codec::Decoder::decode(const string &str, ustring &buf)
{
	for(string::const_iterator i=str.begin(); i!=str.end();)
	{
		UnicodeChar c=decode_char(str, i);
		if(c!=-1)
			buf+=c;
	}
}

ustring Codec::Decoder::decode(const string &str)
{
	ustring buf;
	decode(str, buf);
	return buf;
}

UnicodeChar Codec::Decoder::error(const string &msg)
{
	switch(err_mode)
	{
	case TRANSLITERATE:
		return 0xFFFE;
	case IGNORE_ERRORS:
		return -1;
	default:
		throw CodecError(msg);
	}
}

Codec *create_codec(const string &n)
{
	string name;
	for(string::const_iterator i=n.begin(); i!=n.end(); ++i)
	{
		if(isupper(*i))
			name+=tolower(*i);
		else if(islower(*i) || isdigit(*i))
			name+=*i;
	}

	if(name=="ascii") return new Ascii;
	if(name=="iso2022jp") return new Iso2022Jp;
	if(name=="iso646fi") return new Iso646Fi;
	if(name=="iso88591" || name=="latin1") return new Iso88591;
	if(name=="iso885915" || name=="latin9") return new Iso885915;
	if(name=="jisx0201") return new JisX0201;
	if(name=="jisx0208") return new JisX0208;
	if(name=="utf8") return new Utf8;
	if(name=="windows1252" || name=="cp1252") return new Windows1252;
	throw InvalidParameterValue("Unknown string codec");
}

Codec *detect_codec(const string &str)
{
	bool is_utf8=true;
	bool is_ascii=true;
	bool is_latin1=true;
	unsigned utf8_mb=0;

	for(string::const_iterator i=str.begin(); i!=str.end(); ++i)
	{
		unsigned char c=*i;
		if(c&0x80)
		{
			is_ascii=false;
			if((c&0xC0)==0x80)
			{
				if((c&0xE0)==0x80)
					is_latin1=false;
				if(utf8_mb)
					--utf8_mb;
				else
					is_utf8=false;
			}
			else if((c&0xC0)==0xC0)
			{
				if(utf8_mb)
				{
					is_utf8=false;
					utf8_mb=0;
				}
				else
				{
					for(utf8_mb=1; (c>>(6-utf8_mb))&1; ++utf8_mb) ;
				}
			}
		}
		else if(utf8_mb)
		{
			is_utf8=false;
			utf8_mb=0;
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

} // namespace Codecs
} // namespace Msp
