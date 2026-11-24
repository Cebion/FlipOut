#include <list>
#include <stdexcept>
#include <cctype>
#include <msp/core/algorithm.h>
#include "utils.h"

using namespace std;

namespace {

template<bool long_sep, bool allow_empty>
vector<string> do_split(const string &str, const string &sep, int max_split)
{
	vector<string> result;

	unsigned start = 0;
	while(start<str.size())
	{
		unsigned end = long_sep ? str.find(sep, start) : str.find_first_of(sep, start);
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

		start = end+(long_sep ? sep.size() : 1);

		if(allow_empty && start==str.size())
			result.push_back(string());
	}

	return result;
}

}


namespace Msp {

int strcasecmp(const string &s1, const string &s2)
{
	auto i1 = s1.begin();
	auto i2 = s2.begin();
	for(; (i1!=s1.end() && i2!=s2.end()); ++i1, ++i2)
	{
		const char c1 = ::tolower(*i1);
		const char c2 = ::tolower(*i2);
		if(c1!=c2) return c1-c2;
	}
	if(i1!=s1.end()) return *i1;
	if(i2!=s2.end()) return -*i2;
	return 0;
}

string tolower(const string &str)
{
	string result(str);
	transform(result, [](char c){ return std::tolower(c); });
	return result;
}

string toupper(const string &str)
{
	string result(str);
	transform(result, [](char c){ return std::toupper(c); });
	return result;
}

bool isnumrc(const string &str)
{
	return all_of(str.begin(), str.end(), [](char c){ return std::isdigit(c); });
}

bool isalpha(const string &str)
{
	return all_of(str.begin(), str.end(), [](char c){ return std::isalpha(c); });
}

bool isalnum(const string &str)
{
	return all_of(str.begin(), str.end(), [](char c){ return std::isalnum(c); });
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

string strip(const string &s)
{
	string result = s;
	if(!result.erase(0, result.find_first_not_of(" \t\r\n")).empty())
		result.erase(result.find_last_not_of(" \t\r\n")+1);
	return result;
}

string &append(string &str, const string &sep, const string &other)
{
	if(!str.empty() && !other.empty())
		str += sep;
	str += other;
	return str;
}

string join(const string &str1, const string &sep, const string &str2)
{
	string result = str1;
	return append(result, sep, str2);
}

string c_unescape(const string &str)
{
	bool escape = false;
	unsigned numeric_type = 0;
	unsigned numeric_pos = 0;
	unsigned numeric_value = 0;
	string result;
	for(auto c: str)
	{
		if(numeric_type==16)
		{
			unsigned digit = 0;
			if(c>='0' && c<='9')
				digit = c-'0';
			else if(c>='a' && c<='f')
				digit = c-'a'+10;
			else if(c>='A' && c<='F')
				digit = c-'A'+10;
			else
				throw invalid_argument("c_unescape");

			numeric_value = (numeric_value<<4 | digit);
			++numeric_pos;
			if(numeric_pos==2)
			{
				result += numeric_value;
				numeric_type = 0;
			}
		}
		else if(numeric_type==8)
		{
			unsigned digit = 0;
			if(c>='0' && c<='7')
				digit = c-'0';
			else
				throw invalid_argument("c_unescape");

			numeric_value = (numeric_value<<3 | digit);
			++numeric_pos;
			if(numeric_pos==3)
			{
				result += numeric_value;
				numeric_type = 0;
			}
		}
		else if(escape)
		{
			if(c=='x')
			{
				numeric_type = 16;
				numeric_pos = 0;
				numeric_value = 0;
			}
			else if(c>='0' && c<='3')
			{
				numeric_type = 8;
				numeric_pos = 1;
				numeric_value = c-'0';
			}
			else if(c=='n')
				result += '\n';
			else if(c=='t')
				result += '\t';
			else if(c=='r')
				result += '\r';
			else if(c=='b')
				result += '\b';
			else if(c=='v')
				result += '\v';
			else if(c=='a')
				result += '\a';
			else if(c=='f')
				result += '\f';
			else if(c=='\"')
				result += '\"';
			else if(c=='\'')
				result += '\'';
			else if(c=='\\')
				result += '\\';
			else
				throw invalid_argument("c_unescape");

			escape = false;
		}
		else if(c=='\\')
			escape = true;
		else
			result += c;
	}

	if(escape)	
		throw invalid_argument("c_unescape");

	return result;
}

string c_escape(const string &str, bool escape_8bit)
{
	string result;

	for(char c: str)
	{
		if(c=='\n')
			result += "\\n";
		else if(c=='\t')
			result += "\\t";
		else if(c=='\r')
			result += "\\r";
		else if(c=='\b')
			result += "\\b";
		else if(c=='\v')
			result += "\\v";
		else if(c=='\a')
			result += "\\a";
		else if(c=='\f')
			result += "\\f";
		else if(c=='\"')
			result += "\\\"";
		else if(c=='\'')
			result += "\\\'";
		else if(c=='\\')
			result += "\\\\";
		else if(static_cast<unsigned char>(c)<' ' || (escape_8bit && (c&0x80)))
		{
			char buf[4] = { '\\', 0 };
			for(unsigned j=0; j<3; ++j)
				buf[1+j] = '0'+((static_cast<unsigned char>(c)>>(6-j*3))&7);
			result.append(buf, 4);
		}
		else
			result += c;
	}

	return result;
}

} // namespace Msp
