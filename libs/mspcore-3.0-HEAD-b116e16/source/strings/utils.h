#ifndef MSP_STRINGS_UTILS_H_
#define MSP_STRINGS_UTILS_H_

#include <string>
#include <vector>

namespace Msp {

/** Compares two strings, ignoring upper/lower case.  Returns an integer less
than, equal to or greater than zero depending on whether the first string
lexicographically precedes, is equal to or follows the second one,
respectively. */
int strcasecmp(const std::string &s1, const std::string &s2);

/** Converts a string to lower case. */
std::string tolower(const std::string &);

/** Converts a string to upper case. */
std::string toupper(const std::string &);

/** Checks whether a string consists of digits only. */
bool isnumrc(const std::string &);

/** Checks whether a string consists of alphabetic characters only. */
bool isalpha(const std::string &);

/** Checks whether a string consists of alphanumeric characters only. */
bool isalnum(const std::string &);

/* These are required to make the standard version work from inside the Msp
namespace */
using std::tolower;
using std::toupper;
using std::isalpha;
using std::isalnum;

/** Splits a string at occurrences of any of the characters in sep.  Default
is to split at whitespace.  Two or more consecutive separator characters will
be treated as a single separator.

If max_split is non-negative, at most that many split will be performed, i.e.
the resulting vector will contain at most max_split+1 elements. */
std::vector<std::string> split(const std::string &str, const std::string &sep = " \t\r\n", int max_split = -1);

/** Splits a string on occurrences of a single character. */
std::vector<std::string> split(const std::string &str, char sep, int max_split = -1);

/** Splits a string on occurrences of another string. */
std::vector<std::string> split_long(const std::string &str, const std::string &sep, int max_split = -1);

/** Splits a string on occurrences of another string.  Two consecutive
separators will cause an empty string to be placed in the result. */
std::vector<std::string> split_fields(const std::string &str, const std::string &sep, int max_split = -1);

/** Splits a string on occurrences of a single character.  Two consecutive
separators will cause an empty string to be placed in the result. */
std::vector<std::string> split_fields(const std::string &str, char sep, int max_split = -1);

/** Appends a string to another, using a separator if both are non-empty. */
std::string &append(std::string &str, const std::string &sep, const std::string &other);

/** Joins two strings, using a separator if both are non-empty. */
std::string join(const std::string &str1, const std::string &sep, const std::string &str2);

/** Concatenates strings from an iterator range. */
template<typename Iter>
std::string join(Iter begin, Iter end, const std::string &sep = " ")
{
	std::string result;
	for(Iter i=begin; i!=end; ++i)
		append(result, sep, *i);

	return result;
}

/** Strips leading and trailing whitespace from a string. */
std::string strip(const std::string &);

/** Unescapes a string with C escape sequences. */
std::string c_unescape(const std::string &str);

/** Escapes any non-printable characters in a string with C escape sequences.
Optionally, any characters with the high bit set can be escaped as well. */
std::string c_escape(const std::string &str, bool escape_8bit = true);

} // namespace Msp

#endif
