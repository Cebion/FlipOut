#include <cmath>
#include <cstdint>
#include <limits>
#include "format.h"
#include "lexicalcast.h"

using namespace std;

namespace {

using namespace Msp;

/* Helper to avoid warnings about an unsigned type never being < 0 */
template<typename T, bool f = is_signed<T>::value>
struct IsNegative
{ static bool eval(T v) { return v<0; } };

template<typename T>
struct IsNegative<T, false>
{ static bool eval(T) { return false; } };

template<typename T, bool f = is_signed<T>::value>
struct Negate
{ static T eval(T v) { return -v; } };

template<typename T>
struct Negate<T, false>
{ static T eval(T v) { return (~v)+1; } };

/* Helper to avoid errors about ambiguous function calls since there are no
overloads of abs for unsigned types */
template<typename T, bool f = is_signed<T>::value>
struct Absolute
{ static T eval(T v) { return v<0 ? -v : v; } };

template<typename T>
struct Absolute<T, false>
{ static T eval(T v) { return v; } };


/*** Integer conversions ***/

const char udigits[] = "0123456789ABCDEF";
const char ldigits[] = "0123456789abcdef";

template<typename T>
char *int_to_str(T v, const Fmt &f, char *end)
{
	if(f.get_type()==Fmt::CHAR)
	{
		*--end = v;
		return end;
	}

	char *ptr = end;

	// Find out the base to use
	unsigned base = f.get_base();
	if(!base)
		base = 10;

	// Format the number, starting from the least significant digit
	const char *digits = (f.get_uppercase() ? udigits : ldigits);
	if(v)
	{
		typename std::make_unsigned<T>::type w = Absolute<T>::eval(v);
		while(w)
		{
			*--ptr = digits[w%base];
			w /= base;
		}
	}
	else
		*--ptr = digits[0];

	char sign = (IsNegative<T>::eval(v) ? '-' : f.get_showpos() ? '+' : 0);
	if(f.get_fill()=='0')
	{
		/* Zero-fill, taking base/sign size into account.  The expression is a
		bit ugly, but saves having to write code for creating the prefix both
		ways. */
		unsigned pfxsize = ((f.get_showbase() && base!=10) ? base==8 ? 1 : 2 : 0) + (sign!=0);
		for(unsigned i=(end-ptr)+pfxsize; i<f.get_width(); ++i)
			*--ptr = '0';
	}

	if(f.get_showbase() && v!=0)
	{
		// Add base indicator
		if(base==2)
			*--ptr = (f.get_uppercase() ? 'B' : 'b');
		else if(base==16)
			*--ptr = (f.get_uppercase() ? 'X' : 'x');
		if(base!=10)
			*--ptr = '0';
	}

	if(sign)
		*--ptr = sign;

	return ptr;
}

template<typename T>
string int_to_str(T v, const Fmt &f)
{
	unsigned size = max(f.get_width(), max<unsigned>(f.get_precision(), sizeof(T)*8+3));
	char *buf = new char[size];
	string result(int_to_str(v, f, buf+size), buf+size);
	delete[] buf;
	return result;
}

template<typename T>
T str_to_int(const string &s, const Fmt &f)
{
	if(s.empty())
		throw lexical_error("conversion of '' to integer");

	auto i = s.begin();

	// See if the input starts with a sign
	bool neg = false;
	if(*i=='-')
	{
		if(is_unsigned<T>::value)
			throw lexical_error(format("conversion of '%s' to unsigned integer", s));
		neg = true;
		++i;
	}
	else if(*i=='+')
		++i;

	// Must have some digits to convert
	if(i==s.end())
		throw lexical_error(format("conversion of '%s' to integer", s));

	T base = f.get_base();
	if(!base && i!=s.end())
	{
		// Automatic base detection requested, figure it out
		if(*i=='0' && ++i!=s.end())
		{
			if(*i=='x' || *i=='X')
			{
				base = 16;
				++i;
			}
			else if(*i=='b' || *i=='B')
			{
				base = 2;
				++i;
			}
			else
				base = 8;
		}
		else
			base = 10;
	}

	// Parse the digits
	T result = 0;
	for(; i!=s.end(); ++i)
	{
		T digit = base;
		if(*i>='0' && *i<='9')
			digit = *i-'0';
		else if(*i>='A' && *i<='F')
			digit = *i-'A'+10;
		else if(*i>='a' && *i<='f')
			digit = *i-'a'+10;
		if(digit>=base)
			throw lexical_error(format("conversion of '%s' to integer (base-%d)", s, base));
		T next = result*base+digit;
		if(next/base!=result)
			throw lexical_error(format("conversion of '%s' to %d-bit integer", s, sizeof(T)*8));
		result = next;
	}

	if(neg)
		result = Negate<T>::eval(result);

	return result;
}


/*** Boolean conversions ***/

string bool_to_str(bool b, const Fmt &f)
{
	if(f.get_type()==Fmt::STR)
		return b ? "true" : "false";
	else
		return b ? "1" : "0";
}

bool str_to_bool(const string &s)
{
	if(s.empty())
		throw lexical_error("conversion of '' to boolean");

	if(s=="1" || s=="true" || s=="yes" || s=="on")
		return true;
	else if(s=="0" || s=="false" || s=="no" || s=="off")
		return false;

	throw lexical_error(format("conversion of '%s' to boolean", s));
}


/*** Floating-point conversions ***/

template<typename T>
string flt_to_str(T v, const Fmt &f)
{
	if(f.get_type()==Fmt::CHAR)
		throw format_mismatch("floating-point conversion with character format");

	Fmt::FloatMode mode = f.get_floatmode();
	long double w = abs(v);
	char sign = (v<0 ? '-' : f.get_showpos() ? '+' : 0);

	// Handle infinity and not-a-number as special cases
	if(!(w+w>w) && w!=0)
	{
		string result;
		if(sign)
			result += sign;
		if(!(w>=0))
			result += (f.get_uppercase() ? "NAN" : "nan");
		else
			result += (f.get_uppercase() ? "INF" : "inf");
		if(result.size()<f.get_width())
			result = string(f.get_width()-result.size(), ' ')+result;
		return result;
	}

	/* Find out the base-10 exponent.  Building up the multiplier / divisor
	first helps with accuracy in some cases. */
	int exp = 0;
	if(w>=10)
	{
		long double div = 1;
		while(div*10<=w)
		{
			++exp;
			div *= 10;
		}
		w /= div;
	}
	else if(mode!=Fmt::FIXED && w<1 && w!=0)
	{
		long double mul = 1;
		while(w*mul<1)
		{
			--exp;
			mul *= 10;
		}
		w *= mul;
	}

	// Decide how to format the number
	unsigned digits;
	unsigned leading_zeroes = 0;
	unsigned point = 1;
	bool showexp = false;
	if(mode==Fmt::FIXED)
	{
		point = exp+1;
		digits = point+f.get_precision();
	}
	else if(mode==Fmt::SCI)
	{
		digits = f.get_precision()+1;
		showexp = true;
	}
	else
	{
		digits = max(f.get_precision(), 1U);
		if(exp<-4 || exp>=static_cast<int>(digits))
		{
			point = 1;
			showexp = true;
		}
		else
		{
			if(exp<0)
				leading_zeroes = -exp;
			else
				point = exp+1;
		}
	}

	// Apply rounding
	w += 5.0l/pow(10.0l, static_cast<long double>(digits));
	if(w>=10)
	{
		// Rounding bumped us to the next exponent, deal with it
		w /= 10;
		if(mode==Fmt::AUTOFLT && exp+1==static_cast<int>(digits))
		{
			point = 1;
			showexp = true;
		}
		if(!showexp)
		{
			if(mode==Fmt::FIXED)
				++digits;
			if(leading_zeroes)
				--leading_zeroes;
			else
				++point;
		}
		else
			++exp;
	}

	digits += leading_zeroes;

	// Create a buffer and start from the end
	unsigned size = max(f.get_width(), digits+8);
	char *buf = new char[size];
	char *end = buf+size;
	char *ptr = end;

	// Format exponent
	if(showexp)
	{
		ptr = int_to_str(exp, Fmt().showpos().fill('0').width(3), ptr);
		*--ptr = (f.get_uppercase() ? 'E' : 'e');
	}

	// Format mantissa left-to-right
	char *eptr = ptr;
	ptr -= digits+(point<digits || f.get_showpoint());
	char *mptr = ptr;
	for(unsigned i=0; i<digits; ++i)
	{
		if(i==point)
			*mptr++ = '.';
		if(!leading_zeroes)
		{
			int digit = static_cast<int>(w);
			*mptr++ = '0'+digit;
			w = (w-digit)*10;
		}
		else
		{
			*mptr++ = '0';
			--leading_zeroes;
		}
	}

	if(f.get_showpoint())
	{
		// Radix point requested but not displayed yet, add it
		if(digits<=point)
			*mptr++ = '.';
	}
	else if(mode==Fmt::AUTOFLT && digits>point)
	{
		// Remove trailing zeroes from fraction and a lone radix point
		while(mptr[-1]=='0')
			--mptr;
		if(mptr[-1]=='.')
			--mptr;
		if(mptr!=eptr)
		{
			while(mptr!=ptr)
				*--eptr = *--mptr;
			ptr = eptr;
		}
	}

	// Add filling and sign
	if(f.get_fill()=='0')
	{
		unsigned pfxlen = (sign!=0);
		while(end-ptr+pfxlen<f.get_width())
			*--ptr = '0';
	}
	if(sign)
		*--ptr = sign;

	string result(ptr, end);
	delete[] buf;
	return result;
}

template<typename T>
T str_to_flt(const string &s, const Fmt &)
{
	if(s.empty())
		throw lexical_error("conversion of '' to floating-point");

	auto i = s.begin();

	// See if the input starts with a sign
	bool neg = false;
	if(*i=='-')
	{
		neg = true;
		++i;
	}
	else if(*i=='+')
		++i;

	// Must have some digits to convert
	if(i==s.end())
		throw lexical_error(format("conversion of '%s' to floating-point", s));

	long double v = 0;
	int exp = 0;

	// Parse mantissa
	bool point_seen = false;
	for(; i!=s.end(); ++i)
	{
		if(*i=='.')
		{
			if(point_seen)
				throw lexical_error(format("conversion of '%s' to floating-point", s));
			point_seen = true;
		}
		else if(*i>='0' && *i<='9')
		{
			v = v*10+(*i-'0');
			if(point_seen)
				--exp;
		}
		else if(*i=='e' || *i=='E')
		{
			// We have an exponent
			++i;

			exp += str_to_int<int>(string(i, s.end()), Fmt());
			// str_to_int has eaten the rest of the input or thrown
			break;
		}
		else
			throw lexical_error(format("conversion of '%s' to floating-point", s));
	}

	// Scale and negate the result as needed
	while(exp>0)
	{
		v *= 10;
		--exp;
	}
	while(exp<0)
	{
		v /= 10;
		++exp;
	}

	if(neg)
		v = -v;

	return v;
}


/*** String conversions ***/

string str_to_str(const string &s, const Fmt &f)
{
	if(f.get_type()==Fmt::NUM)
		throw format_mismatch("string conversion with numeric format");
	return s;
}

}

namespace Msp {

void LexicalConverter::result(const string &s)
{
	filled = true;
	if(s.size()<fmt.get_width())
	{
		if(fmt.get_align()==Fmt::RIGHT)
			buf = string(fmt.get_width()-s.size(), fmt.get_fill())+s;
		else
			buf = s+string(fmt.get_width()-s.size(), fmt.get_fill());
	}
	else
		buf = s;
}

const string &LexicalConverter::get() const
{
	if(!filled)
		throw lexical_error("conversion not performed");
	return buf;
}


/*** operator<< ***/

void operator<<(LexicalConverter &c, char v)
{
	Fmt::Type type = c.get_fmt().get_type();
	if(type==Fmt::NUM)
		c.result(int_to_str(v, c.get_fmt()));
	else
		c.result(string(1, v));
}

void operator<<(LexicalConverter &c, signed char v)
{ c.result(int_to_str(v, c.get_fmt())); }

void operator<<(LexicalConverter &c, short v)
{ c.result(int_to_str(v, c.get_fmt())); }

void operator<<(LexicalConverter &c, int v)
{ c.result(int_to_str(v, c.get_fmt())); }

void operator<<(LexicalConverter &c, long v)
{ c.result(int_to_str(v, c.get_fmt())); }

void operator<<(LexicalConverter &c, unsigned char v)
{ c.result(int_to_str(v, c.get_fmt())); }

void operator<<(LexicalConverter &c, unsigned short v)
{ c.result(int_to_str(v, c.get_fmt())); }

void operator<<(LexicalConverter &c, unsigned v)
{ c.result(int_to_str(v, c.get_fmt())); }

void operator<<(LexicalConverter &c, unsigned long v)
{ c.result(int_to_str(v, c.get_fmt())); }

#ifdef __GNUC__
void operator<<(LexicalConverter &c, long long v)
{ c.result(int_to_str(v, c.get_fmt())); }

void operator<<(LexicalConverter &c, unsigned long long v)
{ c.result(int_to_str(v, c.get_fmt())); }
#endif

void operator<<(LexicalConverter &c, bool v)
{ c.result(bool_to_str(v, c.get_fmt())); }

void operator<<(LexicalConverter &c, float v)
{ c.result(flt_to_str(v, c.get_fmt())); }

void operator<<(LexicalConverter &c, double v)
{ c.result(flt_to_str(v, c.get_fmt())); }

void operator<<(LexicalConverter &c, long double v)
{ c.result(flt_to_str(v, c.get_fmt())); }

void operator<<(LexicalConverter &c, const string &s)
{ c.result(str_to_str(s, c.get_fmt())); }

void operator<<(LexicalConverter &c, const char *s)
{ c.result(str_to_str(s, c.get_fmt())); }

void operator<<(LexicalConverter &c, const void *p)
{ c.result(int_to_str(reinterpret_cast<intptr_t>(p), c.get_fmt())); }


/*** operator>> ***/

void operator>>(const LexicalConverter &c, char &v)
{
	if(c.get_fmt().get_type()==Fmt::NUM)
		v = str_to_int<char>(c.get(), c.get_fmt());
	else
	{
		const string &s = c.get();
		if(s.empty())
			throw lexical_error("conversion of '' to character");
		if(s.size()>1)
			throw lexical_error(format("conversion of '%s' to character", s));
		v = s[0];
	}
}

void operator>>(const LexicalConverter &c, signed char &v)
{ v = str_to_int<signed char>(c.get(), c.get_fmt()); }

void operator>>(const LexicalConverter &c, short &v)
{ v = str_to_int<short>(c.get(), c.get_fmt()); }

void operator>>(const LexicalConverter &c, int &v)
{ v = str_to_int<int>(c.get(), c.get_fmt()); }

void operator>>(const LexicalConverter &c, long &v)
{ v = str_to_int<long>(c.get(), c.get_fmt()); }

void operator>>(const LexicalConverter &c, unsigned char &v)
{ v = str_to_int<unsigned char>(c.get(), c.get_fmt()); }

void operator>>(const LexicalConverter &c, unsigned short &v)
{ v = str_to_int<unsigned short>(c.get(), c.get_fmt()); }

void operator>>(const LexicalConverter &c, unsigned int &v)
{ v = str_to_int<unsigned int>(c.get(), c.get_fmt()); }

void operator>>(const LexicalConverter &c, unsigned long &v)
{ v = str_to_int<unsigned long>(c.get(), c.get_fmt()); }

#ifdef __GNUC__
void operator>>(const LexicalConverter &c, long long &v)
{ v = str_to_int<long long>(c.get(), c.get_fmt()); }

void operator>>(const LexicalConverter &c, unsigned long long &v)
{ v = str_to_int<unsigned long long>(c.get(), c.get_fmt()); }
#endif

void operator>>(const LexicalConverter &c, bool &v)
{ v = str_to_bool(c.get()); }

void operator>>(const LexicalConverter &c, float &v)
{ v = str_to_flt<float>(c.get(), c.get_fmt()); }

void operator>>(const LexicalConverter &c, double &v)
{ v = str_to_flt<double>(c.get(), c.get_fmt()); }

void operator>>(const LexicalConverter &c, long double &v)
{ v = str_to_flt<long double>(c.get(), c.get_fmt()); }

void operator>>(const LexicalConverter &c, string &s)
{ s = str_to_str(c.get(), c.get_fmt()); }

} // namespace Msp
