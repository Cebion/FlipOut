/* $Id$

This file is part of libmspstrings
Copyright © 2007 Mikko Rasa
Distributed under the LGPL
*/
#include "glob.h"

using namespace std;

namespace {

template<bool icase>
bool cmp(char c, char h);

template<>
bool cmp<true>(char c, char h)
{ return tolower(c)==tolower(h); }

template<>
bool cmp<false>(char c, char h)
{ return c==h; }

template<bool icase>
bool globmatch(string::const_iterator pat_i, const string::const_iterator &pat_e, string::const_iterator str_i, const string::const_iterator &str_e)
{
	while(pat_i!=pat_e && str_i!=str_e)
	{
		if(*pat_i=='?' || cmp<icase>(*str_i, *pat_i))
		{
			++pat_i;
			++str_i;
		}
		else if(*pat_i=='*')
		{
			++pat_i;
			if(pat_i==pat_e)
				return true;
			
			for(; str_i!=str_e; ++str_i)
				if(cmp<icase>(*str_i, *pat_i) && globmatch<icase>(pat_i, pat_e, str_i, str_e))
					return true;
			
			return false;
		}
		else
			return false;
	}

	return pat_i==pat_e && str_i==str_e;
}

}

namespace Msp {

bool globmatch(const string &pat, const string &str)
{
	return ::globmatch<false>(pat.begin(), pat.end(), str.begin(), str.end());
}

bool globcasematch(const string &pat, const string &str)
{
	return ::globmatch<true>(pat.begin(), pat.end(), str.begin(), str.end());
}

} // namespace Msp
