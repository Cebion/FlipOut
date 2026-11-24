#include "jisx0208.h"

#include "jisx0208.table"

using namespace std;

namespace Msp {
namespace StringCodec {

void JisX0208::Encoder::encode_char(unichar ucs, string &buf)
{
	unsigned short jis = ucs_to_jisx0208(ucs);
	if(jis)
	{
		char jbuf[2];
		jbuf[0] = jis>>8;
		jbuf[1] = jis;
		buf.append(jbuf, 2);
	}
	else
		error(ucs, buf, invalid_character(ucs, "JIS X 0208"));
}

void JisX0208::Encoder::transliterate(unichar, string &buf)
{
	buf.append("!)", 2);
}


unichar JisX0208::Decoder::decode_char(const string &str, string::const_iterator &i)
{
	if(i==str.end())
		return -1;

	auto j = i;
	Kuten jis;
	jis.ku = *j++-0x20;

	unichar result;
	if(j==str.end())
		result = error(invalid_sequence(i, j, "incomplete JIS X 0208 character"));
	else
	{
		jis.ten = *j++-0x20;
		result = jisx0208_to_ucs(jis);
		if(result==-1)
			result = error(invalid_sequence(i, j, "invalid JIS X 0208 ku-ten"));
		if(result==0)
			result = error(invalid_sequence(i, j, "undefined JIS X 0208 character"));
	}

	i = j;
	return result;
}


unichar jisx0208_to_ucs(Kuten jis)
{
	if(jis.ku==0 || jis.ku>0x5E || jis.ten==0 || jis.ten>0x5E)
		return -1;

	return jisx0208_to_ucs_table[jis.ku*94 + jis.ten - 95];
}

Kuten ucs_to_jisx0208(unichar c)
{
	if(c<0 || c>0xFFFF)
		return Kuten();

	unsigned i = 0;
	for(unsigned bit=0x1000; bit; bit>>=1)
	{
		if(i+bit>=ucs_to_jisx0208_table_size)
			continue;
		if(ucs_to_jisx0208_table[i+bit].ucs<=static_cast<unsigned short>(c))
			i += bit;
	}

	Kuten result;
	if(ucs_to_jisx0208_table[i].ucs==static_cast<unsigned short>(c))
	{
		result.ku = (ucs_to_jisx0208_table[i].jis>>8)+1;
		result.ten = ucs_to_jisx0208_table[i].jis+1;
	}

	return result;
}

} // namespace StringCodec
} // namespace Msp
