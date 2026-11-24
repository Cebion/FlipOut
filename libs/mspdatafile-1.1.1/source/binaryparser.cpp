/* $Id: binaryparser.cpp 45 2008-08-06 16:35:21Z tdb $

This file is part of libmspdatafile
Copyright © 2007-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <sys/param.h>
#include <msp/strings/formatter.h>
#include "binaryparser.h"
#include "input.h"

using namespace std;

namespace Msp {
namespace DataFile {

BinaryParser::BinaryParser(Input &i, const string &s):
	ParserMode(i, s),
	first(true)
{
	dict[1]=DictEntry("__kwd", "iss");
	dict[2]=DictEntry("__str", "is");
}

Statement BinaryParser::parse()
{
	while(1)
	{
		Statement st=parse_statement();
		if(st.keyword=="__kwd")
		{
			if(st.args.size()!=3)
				throw_at(TypeError("Keyword definition must have three arguments"), src);

			const unsigned id=st.args[0].get<unsigned>();
			const string &kw=st.args[1].get<const string &>();
			const string &args=st.args[2].get<const string &>();
			dict[id]=DictEntry(kw, args);
		}
		else if(st.keyword=="__str")
		{
			if(st.args.size()!=2)
				throw_at(TypeError("String definition must have two arguments"), src);

			const unsigned id=st.args[0].get<unsigned>();
			strings[id]=st.args[1].get<const string &>();
		}
		else
			return st;
	}
}

Statement BinaryParser::parse_statement()
{
	while(first && in.peek()=='\n')
		in.get();
	first=false;

	unsigned id=parse_int();
	if(!in)
		return Statement();

	Dictionary::const_iterator i=dict.find(id);
	if(i==dict.end())
		throw_at(KeyError("Unknown statement ID", lexical_cast(id)), src);
	const DictEntry &de=i->second;

	Statement result;
	result.keyword=de.keyword;
	result.source=src;

	for(unsigned j=0; j<de.args.size(); ++j)
	{
		switch(de.args[j])
		{
		case 'i':
			result.args.push_back(parse_int());
			break;
		case 'f':
			result.args.push_back(parse_float());
			break;
		case 's':
			result.args.push_back(parse_string());
			break;
		case 'b':
			result.args.push_back(parse_bool());
			break;
		case 'e':
			result.args.push_back(Value(ENUM, parse_enum()));
			break;
		}
	}

	unsigned nsub=parse_int();
	for(unsigned j=0; j<nsub; ++j)
		result.sub.push_back(parse());

	result.valid=true;

	return result;
}

long long BinaryParser::parse_int()
{
	long long result=0;
	unsigned bits=0;

	while(in)
	{
		int c=in.get();

		result=(result<<7) | (c&0x7F);
		bits+=7;

		if(!(c&0x80))
			break;
	}

	const long long mask=1LL<<(bits-1);
	result=(result^mask)-mask;

	return result;
}

float BinaryParser::parse_float()
{
	union
	{
		float f;
		char d[sizeof(float)];
	};

#if BYTE_ORDER == LITTLE_ENDIAN
	for(unsigned i=sizeof(float); i--;)
		d[i]=in.get();
#else
	for(unsigned i=0; i<sizeof(float); ++i)
		d[i]=in.get();
#endif

	return f;
}

bool BinaryParser::parse_bool()
{
	return in.get();
}

string BinaryParser::parse_string()
{
	int len=parse_int();
	if(len>=0)
	{
		string result;
		result.reserve(len);
		for(int i=0; i<len; ++i)
			result+=in.get();
		return result;
	}
	else
		return lookup_string(-len);
}

string BinaryParser::parse_enum()
{
	return lookup_string(parse_int());
}

const string &BinaryParser::lookup_string(unsigned id) const
{
	StringMap::const_iterator i=strings.find(id);
	if(i==strings.end())
		throw_at(KeyError("Unknown string", lexical_cast(id)), src);
	return i->second;
}

} // namespace DataFile
} // namespace Msp
