/* $Id$

This file is part of libmspstrings
Copyright © 2006-2008 Mikko Rasa
Distributed under the LGPL
*/

#ifndef MSP_STRINGS_LEXICALCAST_H_
#define MSP_STRINGS_LEXICALCAST_H_

#include <sstream>
#include <string>
#include <msp/core/except.h>
#include "fmt.h"

namespace Msp {

/**
Thrown for errors in lexical conversions
*/
class LexicalError: public Exception
{
public:
	LexicalError(const std::string &w_): Exception(w_) { }
};

/**
Helper class for lexical_cast to facilitate operator overloading.
*/
class LexicalConverter
{
private:
	Fmt fmt;
	std::string buf;

public:
	LexicalConverter(const Fmt &f): fmt(f) { }
	LexicalConverter(const std::string &s, const Fmt &f): fmt(f), buf(s) { }

	const Fmt &get_fmt() const { return fmt; }
	const std::string &get() const { return buf; }
	void result(const std::string &);
};

void operator<<(LexicalConverter &, char);
void operator<<(LexicalConverter &, signed char);
void operator<<(LexicalConverter &, short);
void operator<<(LexicalConverter &, int);
void operator<<(LexicalConverter &, long);
void operator<<(LexicalConverter &, unsigned char);
void operator<<(LexicalConverter &, unsigned short);
void operator<<(LexicalConverter &, unsigned);
void operator<<(LexicalConverter &, unsigned long);
#ifdef __GNUC__
void operator<<(LexicalConverter &, long long);
void operator<<(LexicalConverter &, unsigned long long);
#endif
void operator<<(LexicalConverter &, bool);
void operator<<(LexicalConverter &, float);
void operator<<(LexicalConverter &, double);
void operator<<(LexicalConverter &, long double);
void operator<<(LexicalConverter &, const std::string &);
void operator<<(LexicalConverter &, const char *);
void operator<<(LexicalConverter &, const void *);

void operator>>(const LexicalConverter &, char &);
void operator>>(const LexicalConverter &, signed char &);
void operator>>(const LexicalConverter &, short &);
void operator>>(const LexicalConverter &, int &);
void operator>>(const LexicalConverter &, long &);
void operator>>(const LexicalConverter &, unsigned char &);
void operator>>(const LexicalConverter &, unsigned short &);
void operator>>(const LexicalConverter &, unsigned int &);
void operator>>(const LexicalConverter &, unsigned long &);
#ifdef __GNUC__
void operator>>(const LexicalConverter &, long long &);
void operator>>(const LexicalConverter &, unsigned long long &);
#endif
void operator>>(const LexicalConverter &, bool &);
void operator>>(const LexicalConverter &, float &);
void operator>>(const LexicalConverter &, double &);
void operator>>(const LexicalConverter &, long double &);
void operator>>(const LexicalConverter &, std::string &);

// Generic operators using stringstream

template<typename T>
void operator<<(LexicalConverter &c, const T &v)
{
	std::ostringstream ss;
	ss<<c.get_fmt()<<v;
	c.result(ss.str());
}

template<typename T>
void operator>>(const LexicalConverter &c, T &v)
{
	std::istringstream ss(c.get());
	ss.setf(std::ios_base::fmtflags(0), std::ios_base::skipws);
	ss>>v;
	if(ss.fail() || !ss.eof())
		throw LexicalError("Conversion failure");
}

// The main interface to the lexical conversion machinery

template<typename T>
inline T lexical_cast(const std::string &s, const Fmt &f=Fmt())
{
	LexicalConverter conv(s, f);
	T result;
	conv>>result;
	return result;
}

template<typename T>
inline std::string lexical_cast(const T &v, const Fmt &f=Fmt())
{
	LexicalConverter conv(f);
	conv<<v;
	return conv.get();
}

} // namespace Msp

#endif
