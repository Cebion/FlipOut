/* $Id$

This file is part of libmspstrings
Copyright © 2006-2008 Mikko Rasa
Distributed under the LGPL
*/

#include <msp/core/except.h>
#include "fmt.h"

using namespace std;

namespace Msp {

/**
Resets the format to the default.  Mainly used by constructors.
*/
Fmt &Fmt::reset()
{
	wd=0;
	prec=6;
	spos=false;
	fillc=' ';
	base=DEC;
	sbase=false;
	fmode=AUTOFLT;
	spoint=false;
	align=RIGHT;
	ucase=false;
	type=STR;

	return *this;
}

/**
Applies the format to the given ostream.  All existing formatting information
is overwritten.
*/
void Fmt::apply(ostream &out) const
{
	out.flags(((base==HEX) ? ios_base::hex : (base==OCT) ? ios_base::oct : ios_base::dec)
		| ((fmode==SCI) ? ios_base::scientific : (fmode==FIXED) ? ios_base::fixed : ios_base::fmtflags(0))
		| (fillc=='0' ? ios_base::internal : (align==LEFT) ? ios_base::left : ios_base::right)
		| (sbase ? ios_base::showbase : ios_base::fmtflags(0))
		| (spoint ? ios_base::showpoint : ios_base::fmtflags(0))
		| (spos ? ios_base::showpos : ios_base::fmtflags(0))
		| (ucase ? ios_base::uppercase : ios_base::fmtflags(0)));
	out.fill(fillc);
	out.width(wd);
	out.precision(prec);
}

/**
Parses a printf-style conversion specification.  Called from constructors.
*/
void Fmt::parse(const char *f)
{
	if(*f=='%') ++f;

	for(; *f; ++f)
	{
		if(*f=='#')
		{
			sbase=true;
			spoint=true;
		}
		else if(*f=='0')
			fillc='0';
		else if(*f=='-')
			align=LEFT;
		else if(*f=='+')
			spos=true;
		else
			break;
	}

	wd=0;
	for(; *f; ++f)
	{
		if(*f>='0' && *f<='9')
			wd=wd*10+(*f-'0');
		else
			break;
	}

	if(*f=='.')
	{
		++f;
		prec=0;
		for(; *f; ++f)
		{
			if(*f>='0' && *f<='9')
				prec=prec*10+(*f-'0');
			else
				break;
		}
	}

	type=NUM;
	if(*f=='d' || *f=='u')
		base=DEC;
	else if(*f=='x' || *f=='X')
		base=HEX;
	else if(*f=='o')
		base=OCT;
	else if(*f=='b')
		base=BIN;
	else if(*f=='e' || *f=='E')
		fmode=SCI;
	else if(*f=='f' || *f=='F')
		fmode=FIXED;
	else if(*f=='g' || *f=='G')
		fmode=AUTOFLT;
	else if(*f=='p' || *f=='P')
	{
		base=HEX;
		sbase=true;
	}
	else if(*f=='c')
		type=CHAR;
	else if(*f=='s')
		type=STR;
	else if(*f=='i')
		base=AUTOBASE;
	else
		throw InvalidParameterValue("Invalid conversion specifier");

	if(*f=='E' || *f=='F' || *f=='G' || *f=='X' || *f=='P')
		ucase=true;

	++f;

	if(*f)
		throw InvalidParameterValue("Extra characters in conversion specification");
}

} // namespace Msp
