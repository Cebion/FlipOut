/* $Id: binarywriter.cpp 43 2008-07-22 15:34:59Z tdb $

This file is part of libmspdatafile
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "binarywriter.h"
#include "statement.h"

using namespace std;

namespace Msp {
namespace DataFile {

BinaryWriter::BinaryWriter(IO::Base &o):
	WriterMode(o),
	next_kwd_id(3),
	next_str_id(1)
{
	dict[DictEntry("__kwd", "iss")]=1;
	dict[DictEntry("__str", "is")]=2;
}

void BinaryWriter::write(const Statement &st)
{
	collect_keywords(st);
	write_(st);
}

void BinaryWriter::write_(const Statement &st)
{
	Dictionary::iterator i=dict.find(create_entry(st));
	if(i==dict.end())
		throw InvalidParameterValue("Unknown statement");

	write_int(i->second);
	for(ValueArray::const_iterator j=st.args.begin(); j!=st.args.end(); ++j)
		switch(j->get_type())
		{
		case INTEGER: write_int   (j->get<long long>()); break;
		case STRING:  write_string(j->get<const string &>()); break;
		case BOOLEAN: write_int   (j->get<bool>()); break;
		case FLOAT:   write_float (j->get<float>()); break;
		case ENUM:    write_enum  (j->get_raw()); break;
		}

	write_int(st.sub.size());
	for(list<Statement>::const_iterator j=st.sub.begin(); j!=st.sub.end(); ++j)
		write(*j);
}

DictEntry BinaryWriter::create_entry(const Statement &st)
{
	static const char types[]="ifsbe";

	string args;
	for(ValueArray::const_iterator i=st.args.begin(); i!=st.args.end(); ++i)
	{
		if(i->get_type()>=5)
			throw InvalidParameterValue("Invalid argument type");
		args+=types[i->get_type()];
	}

	return DictEntry(st.keyword, args);
}

void BinaryWriter::collect_keywords(const Statement &st)
{
	DictEntry de=create_entry(st);

	if(!dict.count(de))
	{
		Statement kst;
		kst.keyword="__kwd";
		kst.args.push_back(next_kwd_id);
		kst.args.push_back(de.keyword);
		kst.args.push_back(de.args);
		write_(kst);

		dict[de]=next_kwd_id++;
	}

	for(ValueArray::const_iterator i=st.args.begin(); i!=st.args.end(); ++i)
	{
		const Type t=i->get_type();
		const string &r=i->get_raw();
		if((t==ENUM || (t==STRING && r.size()<32)) && !strings.count(r))
		{
			Statement sst;
			sst.keyword="__str";
			sst.args.push_back(next_str_id);
			sst.args.push_back(r);
			write_(sst);

			strings[r]=next_str_id++;
		}
	}

	for(list<Statement>::const_iterator i=st.sub.begin(); i!=st.sub.end(); ++i)
		collect_keywords(*i);
}

void BinaryWriter::write_int(long long n)
{
	unsigned i=sizeof(long long)-1;

	if(n>=0)
		for(; (i>0 && (n>>(i*7-1))==0); --i) ;
	else
		for(; (i>0 && (n>>(i*7-1))==-1); --i) ;

	for(++i; i--;)
		out.put((n>>(i*7) & 0x7F) | (i?0x80:0));
}

void BinaryWriter::write_string(const string &s)
{
	StringMap::const_iterator i=strings.find(s);
	if(i!=strings.end())
		write_int(-static_cast<int>(i->second));
	else
	{
		write_int(s.size());
		out.write(s.data(), s.size());
	}
}

void BinaryWriter::write_float(float f)
{
	union
	{
		float v;
		char d[sizeof(float)];
	};

	v=f;
#if BYTE_ORDER == LITTLE_ENDIAN
	for(unsigned i=sizeof(float); i--;)
		out.put(d[i]);
#else
	for(unsigned i=0; i<sizeof(float); ++i)
		out.put(d[i]);
#endif
}

void BinaryWriter::write_enum(const string &e)
{
	StringMap::const_iterator i=strings.find(e);
	if(i==strings.end())
		throw InvalidParameterValue("Unknown enum");
	write_int(i->second);
}

} // namespace DataFile
} // namespace Msp
