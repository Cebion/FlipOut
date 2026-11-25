/* $Id$

This file is part of libmspstrings
Copyright © 2006-2008 Mikko Rasa
Distributed under the LGPL
*/

#ifndef MSP_STRINGS_FMT_H_
#define MSP_STRINGS_FMT_H_

#include <ostream>
#include <string>

namespace Msp {

/**
Stores formatting information for converting variables into strings.  Can be
applied to an std::ostream or fed to lexical_cast.  Also used internally by
Formatter.

Formats can be constructed from printf-style conversion specifications, by
chaining calls to the various setter functions, or with a mixture of both.

Since type information for conversions is acquired through templates, the
meaning of the conversion specifier character is reduced to only specifying
what the conversion should look like.  Of special note is the s conversion,
which will result in a default conversion for any data type.  Size modifiers
are not supported and there is no difference between signed and unsigned
conversions.

Some new conversions are supported:

  b/B  Binary integer conversion
  P    Uppercase pointer conversion (like %#X)
*/
class Fmt
{
public:
	enum Type
	{
		NUM,
		CHAR,
		STR
	};

	enum Base
	{
		AUTOBASE = 0,
		DEC = 10,
		HEX = 16,
		OCT = 8,
		BIN = 2
	};

	enum FloatMode
	{
		FIXED,
		AUTOFLT,
		SCI
	};

	enum Align
	{
		LEFT,
		RIGHT
	};

private:
	unsigned  wd;
	unsigned  prec;
	bool      spos;
	wchar_t   fillc;
	Base      base;
	bool      sbase;
	FloatMode fmode;
	bool      spoint;
	Align     align;
	bool      ucase;
	Type      type;

public:
	Fmt()                     { reset(); }
	Fmt(const char *f)        { reset(); parse(f); }
	Fmt(const std::string &f) { reset(); parse(f.c_str()); }

	Fmt &width(unsigned w)      { wd=w; return *this; }
	Fmt &precision(unsigned p)  { prec=p; return *this; }
	Fmt &showpos(bool s=true)   { spos=s; return *this; }
	Fmt &fill(wchar_t f)        { fillc=f; return *this; }
	Fmt &fixed()                { fmode=FIXED; return *this; }
	Fmt &scientific()           { fmode=SCI; return *this; }
	Fmt &showpoint(bool s=true) { spoint=s; return *this; }
	Fmt &showbase(bool s=true)  { sbase=s; return *this; }
	Fmt &left()                 { align=LEFT; return *this; }
	Fmt &right()                { align=RIGHT; return *this; }
	Fmt &dec()                  { base=DEC; return *this; }
	Fmt &hex()                  { base=HEX; return *this; }
	Fmt &oct()                  { base=OCT; return *this; }
	Fmt &bin()                  { base=BIN; return *this; }
	Fmt &uppercase(bool u=true) { ucase=u; return *this; }
	Fmt &numeric()              { type=NUM; return *this; }
	Fmt &character()            { type=CHAR; return *this; }
	Fmt &string()               { type=STR; return *this; }
	Fmt &reset();

	unsigned  get_width() const     { return wd; }
	unsigned  get_precision() const { return prec; }
	bool      get_showpos() const   { return spos; }
	wchar_t   get_fill() const      { return fillc; }
	Base      get_base() const      { return base; }
	bool      get_showbase() const  { return sbase; }
	FloatMode get_floatmode() const { return fmode; }
	bool      get_showpoint() const { return spoint; }
	Align     get_align() const     { return align; }
	bool      get_uppercase() const { return ucase; }
	Type      get_type() const      { return type; }

	void apply(std::ostream &) const;
private:
	void parse(const char *);
};

inline std::ostream &operator<<(std::ostream &o, const Fmt &f)
{ f.apply(o); return o; }

} // namespace Msp

#endif
