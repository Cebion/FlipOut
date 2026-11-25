/* $Id$

This file is part of libmspstrings
Copyright © 2006-2007 Mikko Rasa
Distributed under the LGPL
*/

#ifndef MSP_STRINGS_FORMATTER_H_
#define MSP_STRINGS_FORMATTER_H_

#include <string>
#include "lexicalcast.h"

namespace Msp {

/**
Printf-like string formatter class.
*/
class Formatter
{
public:
	Formatter(const std::string &);

	/**
	Extracts the next conversion from the format string and formats the given
	value with it.  Will throw if no more conversions are found.
	*/
	template<typename T>
	Formatter &operator()(const T &a)
	{
		result+=lexical_cast(a, get_conversion());
		advance();
		return *this;
	}

	const std::string &str() const;
private:
	std::string fmt;
	std::string::iterator pos;
	std::string result;

	void advance();
	Fmt  get_conversion();
};

inline Formatter format(const std::string &f)
{ return Formatter(f); }

template<typename A1>
inline std::string format(const std::string &f, const A1 &a1)
{ return Formatter(f)(a1).str(); }

template<typename A1, typename A2>
inline std::string format(const std::string &f, const A1 &a1, const A2 &a2)
{ return Formatter(f)(a1)(a2).str(); }

template<typename A1, typename A2, typename A3>
inline std::string format(const std::string &f, const A1 &a1, const A2 &a2, const A3 &a3)
{ return Formatter(f)(a1)(a2)(a3).str(); }

template<typename A1, typename A2, typename A3, typename A4>
inline std::string format(const std::string &f, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4)
{ return Formatter(f)(a1)(a2)(a3)(a4).str(); }

template<typename A1, typename A2, typename A3, typename A4, typename A5>
inline std::string format(const std::string &f, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5)
{ return Formatter(f)(a1)(a2)(a3)(a4)(a5).str(); }

} // namespace Msp

#endif
