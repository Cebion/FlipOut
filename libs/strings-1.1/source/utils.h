/* $Id$

This file is part of libmspstrings
Copyright © 2006-2008 Mikko Rasa
Distributed under the LGPL
*/

#ifndef MSP_STRINGS_UTILS_H_
#define MSP_STRINGS_UTILS_H_

#include <string>
#include <vector>

namespace Msp {

/**
Compares two strings, ignoring upper/lower case.

@param   s1  First string
@param   s2  Second string

@return  -1 if s1<s2, 0 if s1==s2, 1 if s1>s2
*/
int strcasecmp(const std::string &s1, const std::string &s2);

/**
Converts a string to lower case.
*/
std::string tolower(const std::string &);

/**
Converts a string to upper case.
*/
std::string toupper(const std::string &);

/**
Checks whether a string consists of digits only.
*/
bool isnumrc(const std::string &);

/**
Checks whether a string consists of alphabetic characters only.
*/
bool isalpha(const std::string &);

/**
Checks whether a string consists of alphanumeric characters only.
*/
bool isalnum(const std::string &);

/* These are required to make the standard version work from inside the Msp
namespace */
using std::tolower;
using std::toupper;
using std::isalpha;
using std::isalnum;

/**
Splits a string at occurrences of any of the characters in sep.  If max_split
is non-negative, at most that many split will be performed, i.e. the resulting
vector will contain at most max_split+1 elements.  Two or more consecutive
separator characters will be treated as a single separator.

@param   str        A string
@param   sep        Separator characters
@param   max_split  Maximum number of splits to perform
*/
std::vector<std::string> split(const std::string &str, const std::string &sep=" \t\r\n", int max_split=-1);

/**
Splits a string on occurrences of a single character.
*/
std::vector<std::string> split(const std::string &str, char sep, int max_split=-1);

/**
Splits a string on occurrences of another string.
*/
std::vector<std::string> split_long(const std::string &str, const std::string &sep, int max_split=-1);

/**
Splits a string on occurrences of another string.  Two consecutive separators
will cause an empty string to be placed in the result.
*/
std::vector<std::string> split_fields(const std::string &str, const std::string &sep, int max_split=-1);

/**
Splits a string on occurrences of a single character.  Two consecutive
separators will cause an empty string to be placed in the result.
*/
std::vector<std::string> split_fields(const std::string &str, char sep, int max_split=-1);

/**
Concatenates strings from an iterator range.

@param  begin  First iterator
@param  end    Last iterator
@param  sep    Separator to be inserted between strings
*/
template<typename Iter>
std::string join(Iter begin, Iter end, const std::string &sep=" ")
{
	std::string result;
	for(Iter i=begin; i!=end; ++i)
	{
		if(i!=begin)
			result+=sep;
		result+=*i;
	}

	return result;
}

/**
Strips leading and trailing whitespace from a string.
*/
std::string strip(const std::string &);

/**
Unescapes a string with C escape sequences.
*/
std::string c_unescape(const std::string &str);

/**
Escapes any non-printable characters in a string with C escape sequences.

@param   str          A string
@param   escape_8bit  If true, consider characters with high bit set as
                      non-printable

@return  An escaped version of the string
*/
std::string c_escape(const std::string &str, bool escape_8bit=true);

} // namespace Msp

#endif
