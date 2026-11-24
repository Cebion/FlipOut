#ifndef MSP_STRINGS_LEXICALCAST_H_
#define MSP_STRINGS_LEXICALCAST_H_

#include <sstream>
#include <string>
#include <stdexcept>
#include <msp/core/meta.h>
#include "fmt.h"

namespace Msp {

/**
Thrown for errors in lexical conversions.
*/
class lexical_error: public std::runtime_error
{
public:
	lexical_error(const std::string &w): runtime_error(w) { }
	virtual ~lexical_error() throw() { }
};


/**
Thrown when the format is unsuitable for the type being converted.
*/
class format_mismatch: public lexical_error
{
public:
	format_mismatch(const std::string &w): lexical_error(w) { }
	virtual ~format_mismatch() throw() { }
};


/**
Helper class for lexical_cast to facilitate operator overloading.
*/
class LexicalConverter
{
private:
	Fmt fmt;
	bool filled;
	std::string buf;

public:
	LexicalConverter(const Fmt &f): fmt(f), filled(false) { }
	LexicalConverter(const std::string &s, const Fmt &f): fmt(f), filled(true), buf(s) { }

	const Fmt &get_fmt() const { return fmt; }
	const std::string &get() const;
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

struct CheckFormattedOutput: Sfinae
{
	static std::ostream &s;
	template<typename T>
	static Yes f(int (*)[sizeof(s<<reinterpret_cast<const T &>(s))]);
	using Sfinae::f;
};

struct CheckFormattedInput: Sfinae
{
	static std::istream &s;
	template<typename T>
	static Yes f(int (*)[sizeof(s>>reinterpret_cast<T &>(s))]);
	using Sfinae::f;
};

template<typename T> struct HasFormattedOutput: Sfinae::Evaluate<CheckFormattedOutput, T> { };
template<typename T> struct HasFormattedInput: Sfinae::Evaluate<CheckFormattedInput, T> { };


template<typename T>
typename std::enable_if<HasFormattedOutput<T>::value>::type
operator<<(LexicalConverter &c, const T &v)
{
	std::ostringstream ss;
	ss << c.get_fmt() << v;
	c.result(ss.str());
}

template<typename T>
typename std::enable_if<HasFormattedInput<T>::value>::type
operator>>(const LexicalConverter &c, T &v)
{
	std::istringstream ss(c.get());
	ss.setf(std::ios_base::fmtflags(0), std::ios_base::skipws);
	ss >> v;
	if(ss.fail() || !ss.eof())
		throw lexical_error("conversion failure");
}

/**
Helper struct to provide partial template specialization.
*/
template<typename T, typename F>
struct LexicalCast;

template<typename T>
struct LexicalCast<T, std::string>
{
	static T cast(const std::string &s, const Fmt &f = Fmt())
	{
		LexicalConverter conv(s, f);
		T result;
		conv >> result;
		return result;
	}
};

template<typename F>
struct LexicalCast<std::string, F>
{
	static std::string cast(const F &v, const Fmt &f = Fmt())
	{
		LexicalConverter conv(f);
		conv << v;
		return conv.get();
	}
};

template<>
struct LexicalCast<std::string, std::string>
{
	static std::string cast(const std::string &v, const Fmt &f = Fmt())
	{
		LexicalConverter conv(f);
		conv << v;
		return conv.get();
	}
};

/** Perform a lexical conversion between a string and another type.  The source
type can normally be deduced by the compiler, so this can be used just like the
standard C++ casts.  A format may additionally be specified to force a specific
interpretation. */
template<typename T, typename F>
inline T lexical_cast(const F &v, const Fmt &f = Fmt())
{
	return LexicalCast<T, F>::cast(v, f);
}

} // namespace Msp

#endif
