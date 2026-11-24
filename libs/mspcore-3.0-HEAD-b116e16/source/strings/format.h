#ifndef MSP_STRINGS_FORMAT_H_
#define MSP_STRINGS_FORMAT_H_

#include <string>
#include "lexicalcast.h"

namespace Msp {

/**
Printf-like string formatter class.
*/
class Formatter
{
private:
	std::string fmt;
	std::string::iterator pos;
	std::string result;

public:
	Formatter(const std::string &);
	Formatter(const Formatter &);
	Formatter &operator=(const Formatter &);

	/** Extracts the next conversion from the format string and formats the
	given value with it.  Will throw if no more conversions are found. */
	template<typename T>
	Formatter &operator()(const T &a)
	{
		result += lexical_cast<std::string>(a, get_conversion());
		advance();
		return *this;
	}

	template<typename T, typename... Tail>
	Formatter &operator()(const T &a, const Tail &... tail)
	{
		return (*this)(a)(tail...);
	}

	/** Returns the result of the formatting operation.  Will throw if not
	enough values have been fed to the formatter. */
	const std::string &str() const;

private:
	/** Advances the iterator to the next conversion, adding literal characters
	to the result.  The iterator is left at the second character of the
	conversion (i.e. after the %). */
	void advance();

	/** Reads the next conversion from the format string and returns a
	corresponding Fmt object. */
	Fmt get_conversion();
};

inline Formatter format(const std::string &f)
{ return Formatter(f); }

template<typename... Args>
inline std::string format(const std::string &f, const Args &... args)
{
	return Formatter(f)(args...).str();
}

} // namespace Msp

#endif
