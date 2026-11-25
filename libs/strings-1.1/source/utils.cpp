/* $Id$

This file is part of libmspstrings
Copyright © 2006-2008 Mikko Rasa
Distributed under the LGPL
*/

#include <algorithm>
#include <list>
#include <msp/core/except.h>
#include "utils.h"

using namespace std;

namespace {

template<bool long_sep, bool allow_empty>
vector<string> do_split(const string &str, const string &sep, int max_split)
{
	vector<string> result;

	unsigned start=0;
	while(start<str.size())
	{
		unsigned end=long_sep ? str.find(sep, start) : str.find_first_of(sep, start);
		if(end!=start || allow_empty)
		{
			if(max_split>=0 && result.size()==static_cast<unsigned>(max_split))
			{
				result.push_back(str.substr(start));
				break;
			}
			else
				result.push_back(str.substr(start, end-start));
		}

		if(end>str.size())
			break;

		start=end+(long_sep ? sep.size() : 1);

		if(allow_empty && start==str.size())
			result.push_back(string());
	}

	return result;
}

bool check_str(const std::string &str, int (*pred)(int))
{
	for(string::const_iterator i=str.begin(); i!=str.end(); ++i)
		if(!pred(*i))
			return false;
	return true;
}

}

namespace Msp {

int strcasecmp(const string &s1, const string &s2)
{
	string::const_iterator i1=s1.begin();
	string::const_iterator i2=s2.begin();
	for(; (i1!=s1.end() && i2!=s2.end()); ++i1, ++i2)
	{
		const char c1=::tolower(*i1);
		const char c2=::tolower(*i2);
		if(c1!=c2) return c1-c2;
	}
	if(i1!=s1.end()) return *i1;
	if(i2!=s2.end()) return -*i2;
	return 0;
}

string tolower(const string &str)
{
	string result(str);
	transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}

string toupper(const string &str)
{
	string result(str);
	transform(result.begin(), result.end(), result.begin(), ::toupper);
	return result;
}

bool isnumrc(const string &str)
{
	return check_str(str, isdigit);
}

bool isalpha(const string &str)
{
	return check_str(str, isalpha);
}

bool isalnum(const string &str)
{
	return check_str(str, isalnum);
}

vector<string> split(const string &str, const string &sep, int max_split)
{
	return do_split<false, false>(str, sep, max_split);
}

vector<string> split(const string &str, char sep, int max_split)
{
	return split(str, string(1, sep), max_split);
}

vector<string> split_long(const string &str, const string &sep, int max_split)
{
	return do_split<true, false>(str, sep, max_split);
}

vector<string> split_fields(const string &str, const string &sep, int max_split)
{
	return do_split<true, true>(str, sep, max_split);
}

vector<string> split_fields(const string &str, char sep, int max_split)
{
	return split_fields(str, string(1, sep), max_split);
}

/**
Splits a string to parts.

@param   str          String to be split
@param   sep          A set of separator characters
@param   allow_empty  Whether or not to produce empty parts for sequences of
                      more than one separator character
*/
vector<string> split(const string &str, const string &sep, bool allow_empty)
{
	vector<string> result;
	
	string::size_type start=0;
	if(!allow_empty)
		start=str.find_first_not_of(sep);
	
	while(start<str.size())
	{
		string::size_type end=str.find_first_of(sep, start);
		result.push_back(str.substr(start, end-start));
		
		if(end==string::npos)
			break;
		
		if(allow_empty)
		{
			start=end+1;
			if(start==str.size())
				result.push_back(string());
		}
		else
			start=str.find_first_not_of(sep, end);
	}

	return result;
}

vector<string> split(const string &str, char sep, bool allow_empty)
{
	return split(str, string(1, sep), allow_empty);
}

string strip(const string &s)
{
	string result=s;
	if(!result.erase(0, result.find_first_not_of(" \t\r\n")).empty())
		result.erase(result.find_last_not_of(" \t\r\n")+1);
	return result;
}

string c_unescape(const std::string &str)
{
	bool escape=false;
	unsigned numeric_type=0;
	unsigned numeric_pos=0;
	unsigned numeric_value=0;
	string result;
	for(string::const_iterator i=str.begin(); i!=str.end(); ++i)
	{
		if(numeric_type==16)
		{
			unsigned digit=0;
			if(*i>='0' && *i<='9')
				digit=*i-'0';
			else if(*i>='a' && *i<='f')
				digit=*i-'a'+10;
			else if(*i>='A' && *i<='F')
				digit=*i-'A'+10;
			else
				throw InvalidParameterValue("Invalid hexadecimal digit");

			numeric_value=(numeric_value<<4 | digit);
			++numeric_pos;
			if(numeric_pos==2)
			{
				result+=numeric_value;
				numeric_type=0;
			}
		}
		else if(numeric_type==8)
		{
			unsigned digit=0;
			if(*i>='0' && *i<='7')
				digit=*i-'0';
			else
				throw InvalidParameterValue("Invalid octal digit");

			numeric_value=(numeric_value<<3 | digit);
			++numeric_pos;
			if(numeric_pos==3)
			{
				result+=numeric_value;
				numeric_type=0;
			}
		}
		else if(escape)
		{
			if(*i=='x')
			{
				numeric_type=16;
				numeric_pos=0;
				numeric_value=0;
			}
			else if(*i>='0' && *i<='3')
			{
				numeric_type=8;
				numeric_pos=1;
				numeric_value=*i-'0';
			}
			else if(*i=='n')
				result+='\n';
			else if(*i=='t')
				result+='\t';
			else if(*i=='r')
				result+='\r';
			else if(*i=='b')
				result+='\b';
			else if(*i=='v')
				result+='\v';
			else if(*i=='a')
				result+='\a';
			else if(*i=='f')
				result+='\f';
			else if(*i=='\"')
				result+='\"';
			else if(*i=='\'')
				result+='\'';
			else if(*i=='\\')
				result+='\\';
			else
				throw InvalidParameterValue("Invalid escape sequence");

			escape=false;
		}
		else if(*i=='\\')
			escape=true;
		else
			result+=*i;
	}

	if(escape)	
		throw InvalidParameterValue("Stray backslash at end of string");

	return result;
}

string c_escape(const string &str, bool escape_8bit)
{
	string result;

	for(string::const_iterator i=str.begin(); i!=str.end(); ++i)
	{
		if(*i=='\n')
			result+="\\n";
		else if(*i=='\t')
			result+="\\t";
		else if(*i=='\r')
			result+="\\r";
		else if(*i=='\b')
			result+="\\b";
		else if(*i=='\v')
			result+="\\v";
		else if(*i=='\a')
			result+="\\a";
		else if(*i=='\f')
			result+="\\f";
		else if(*i=='\"')
			result+="\\\"";
		else if(*i=='\'')
			result+="\\\'";
		else if(*i=='\\')
			result+="\\\\";
		else if(static_cast<unsigned char>(*i)<' ' || (escape_8bit && (*i&0x80)))
		{
			char buf[4]={'\\', '0'+((*i>>6)&3), '0'+((*i>>3)&7), '0'+(*i&7)};
			result.append(buf, 4);
		}
		else
			result+=*i;
	}

	return result;
}

} // namespace Msp
