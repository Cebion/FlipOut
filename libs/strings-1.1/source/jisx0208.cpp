/* $Id$

This file is part of libmspstrings
Copyright © 2006-2007 Mikko Rasa
Distributed under the LGPL
*/

#include "jisx0208.h"

#include "jisx0208.table"

using namespace std;

namespace Msp {
namespace Codecs {

void JisX0208::Encoder::encode_char(UnicodeChar ucs, string &buf)
{
	unsigned short jis=ucs_to_jisx0208(ucs);
	if(jis)
	{
		char jbuf[2]={jis>>8, jis};
		buf.append(jbuf, 2);
	}
	else
		error(ucs, buf, "Can't express character in JIS X 0208");
}

void JisX0208::Encoder::transliterate(UnicodeChar, string &buf)
{
	buf.append("!)", 2);
}


UnicodeChar JisX0208::Decoder::decode_char(const string &str, string::const_iterator &i)
{
	if(i==str.end())
		return error("No input");

	string::const_iterator j=i;
	Kuten jis;
	jis.ku=*j++-0x20;

	UnicodeChar result;
	if(j==str.end())
		result=error("Incomplete JIS X 0208 character");
	else
	{
		jis.ten=*j++-0x20;
		result=jisx0208_to_ucs(jis);
		if(result==0)
			result=error("Undefined JIS X 0208 character");
	}

	i=j;
	return result;
}


UnicodeChar jisx0208_to_ucs(Kuten jis)
{
	if(jis.ku==0 || jis.ku>0x5E || jis.ten==0 || jis.ten>0x5E)
		return 0;

	return jisx0208_to_ucs_table[jis.ku*94 + jis.ten - 95];
}

Kuten ucs_to_jisx0208(UnicodeChar c)
{
	if(c<0 || c>0xFFFF)
		return Kuten();

	unsigned i=0;
	for(unsigned bit=0x1000; bit; bit>>=1)
	{
		if(i+bit>=ucs_to_jisx0208_table_size)
			continue;
		if(ucs_to_jisx0208_table[i+bit].ucs<=static_cast<unsigned short>(c))
			i+=bit;
	}

	Kuten result;
	if(ucs_to_jisx0208_table[i].ucs==static_cast<unsigned short>(c))
	{
		result.ku=(ucs_to_jisx0208_table[i].jis>>8)+1;
		result.ten=ucs_to_jisx0208_table[i].jis+1;
	}

	return result;
}

} // namespace Codecs
} // namespace Msp
