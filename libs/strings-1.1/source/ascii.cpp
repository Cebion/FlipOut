/* $Id$

This file is part of libmspstrings
Copyright © 2006-2007 Mikko Rasa
Distributed under the LGPL
*/

#include "ascii.h"

using namespace std;

/*namespace {

char translit_latin1[0x60]=
{
};

const char *translit_katakana[0x60]=
{
	"--", "a", "a", "i", "i", "u", "u", "e", "e", "o", "o",
	"ka", "ga", "ki", "gi", "ku", "gu", "ke", "ge", "ko", "go",
	"sa", "za", "si", "zi", "su", "zu", "se", "ze", "so", "zo",
	"ta", "da", "ti", "di", "tu", "tu", "du", "te", "de", "to", "do",
};

}*/

namespace Msp {
namespace Codecs {

void Ascii::Encoder::encode_char(UnicodeChar ch, string &buf)
{
	if(ch<0 || ch>0x7F)
		return error(ch, buf, "Can't express character in ASCII");

	buf+=ch;
}

void Ascii::Encoder::transliterate(UnicodeChar ch, string &buf)
{
	if(ch>=0xC0 && ch<=0xC5)
		buf+='A';
	else if(ch==0xC6)
		buf+="AE";
	else if(ch==0xC7)
		buf+='C';
	else if(ch>=0xC8 && ch<=0xCB)
		buf+='E';
	else if(ch>=0xCC && ch<=0xCF)
		buf+='I';
	else if(ch==0xD0)
		buf+='D';
	else if(ch==0xD1)
		buf+='N';
	else if((ch>=0xD2 && ch<=0xD7) || ch==0xD9)
		buf+='O';
	else if(ch==0xD8)
		buf+='x';
	else if(ch>=0xDA && ch<=0xDC)
		buf+='U';
	else if(ch==0xDD)
		buf+='Y';
	else if(ch==0xDE)
		buf+='T';
	else if(ch==0xDF)
		buf+="ss";
	else if(ch>=0xE0 && ch<=0xE5)
		buf+='a';
	else if(ch==0xE6)
		buf+="ae";
	else if(ch==0xE7)
		buf+='c';
	else if(ch>=0xE8 && ch<=0xEB)
		buf+='e';
	else if(ch>=0xEC && ch<=0xEF)
		buf+='i';
	else if(ch==0xF0)
		buf+='d';
	else if(ch==0xF1)
		buf+='n';
	else if((ch>=0xF2 && ch<=0xF7) || ch==0xF9)
		buf+='o';
	else if(ch==0xF8)
		buf+='/';
	else if(ch>=0xFA && ch<=0xFC)
		buf+='u';
	else if(ch==0xFD)
		buf+='y';
	else if(ch==0xFE)
		buf+='t';
	else if(ch==0xFF)
		buf+='y';
	else
		buf+='?';
}


UnicodeChar Ascii::Decoder::decode_char(const string &str, string::const_iterator &i)
{
	if(i==str.end())
		return error("No input");
	else if(*i&0x80)
	{
		UnicodeChar result=error("Undefined ASCII character");
		++i;
		return result;
	}
	else
		return *i++;
}

} // namespace Codecs
} // namespace Msp
