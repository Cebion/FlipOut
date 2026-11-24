/* $Id: compiler.cpp 53 2008-09-13 18:03:04Z tdb $

This file is part of libmspdatafile
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/datafile/parser.h>
#include <msp/fs/dir.h>
#include <msp/fs/utils.h>
#include <msp/strings/regex.h>
#include "compiler.h"

using namespace std;
using namespace Msp;

Compiler::Compiler(DataFile::Writer &w):
	writer(w),
	reset_src(false)
{
	add("file",     &Compiler::file);
	add("for_each", &Compiler::for_each);
	add("write",    &Compiler::write);
}

void Compiler::file(const string &fn)
{
	File fl(*this, FS::dirname(get_source())/fn);
	load_sub_with(fl);
}

void Compiler::for_each(const vector<string> &patterns)
{
	ForEach fe(*this, FS::dirname(get_source()), list<string>(patterns.begin(), patterns.end()));
	load_sub_with(fe);
}

void Compiler::write(const DataFile::Statement &st)
{
	if(reset_src)
	{
		writer.write((DataFile::Statement("__src"), string()));
		reset_src=false;
	}

	for(list<DataFile::Statement>::const_iterator i=st.sub.begin(); i!=st.sub.end(); ++i)
		writer.write(*i);
}

bool Compiler::process_statement(const FS::Path &fn, DataFile::Statement &st)
{
	if(st.keyword=="_content")
		return true;

	for(vector<DataFile::Value>::iterator i=st.args.begin(); i!=st.args.end(); ++i)
		if(i->get_type()==DataFile::STRING)
		{
			if(i->get_raw()=="$filename")
				*i=DataFile::Value(FS::basename(fn.str()));
			else if(i->get_raw()=="$content")
			{
				IO::File in(fn.str());
				string data;
				while(!in.eof())
				{
					char buf[4096];
					unsigned len=in.read(buf, sizeof(buf));
					data.append(buf, len);
				}
				*i=DataFile::Value(data);
			}
		}

	for(list<DataFile::Statement>::iterator i=st.sub.begin(); i!=st.sub.end();)
	{
		if(process_statement(fn, *i))
		{
			IO::File in(fn.str());
			IO::Buffered buf(in);

			DataFile::Parser parser(in, fn.str());
			while(parser)
			{
				DataFile::Statement ss=parser.parse();
				if(ss.valid)
					st.sub.insert(i, ss);
			}
			i=st.sub.erase(i);
		}
		else
			++i;
	}

	return false;
}

void Compiler::process_file(const FS::Path &fn, const list<DataFile::Statement> &st)
{
	writer.write((DataFile::Statement("__src"), FS::basename(fn.str())));
	reset_src=true;

	if(st.empty())
		process_file(fn);
	else
	{
		for(list<DataFile::Statement>::const_iterator i=st.begin(); i!=st.end(); ++i)
		{
			if(i->keyword=="_content")
				process_file(fn);
			else
			{
				DataFile::Statement s=*i;
				process_statement(fn, s);
				writer.write(s);
			}
		}
	}
}

void Compiler::process_file(const FS::Path &fn)
{
	IO::File in(fn.str());
	IO::Buffered buf(in);

	DataFile::Parser parser(in, fn.str());
	while(parser)
	{
		DataFile::Statement st=parser.parse();
		if(st.valid)
			writer.write(st);
	}
}


File::File(Compiler &c, const FS::Path &fn):
	compiler(c),
	filename(fn)
{
	add("write", &File::write);
}

void File::finish()
{
	compiler.process_file(filename, write_st);
}

void File::write(const DataFile::Statement &st)
{
	write_st.insert(write_st.end(), st.sub.begin(), st.sub.end());
}


ForEach::ForEach(Compiler &c, const FS::Path &b, const list<string> &p):
	compiler(c),
	base(b),
	patterns(p)
{
	add("exclude", &ForEach::exclude);
	add("pattern", &ForEach::pattern);
	add("write",   &ForEach::write);
}

void ForEach::finish()
{
	list<string> files=FS::list_files(base);
	for(list<string>::iterator i=files.begin(); i!=files.end(); ++i)
	{
		bool match=false;
		for(list<string>::const_iterator j=patterns.begin(); (j!=patterns.end() && !match); ++j)
			match=Regex(*j).match(*i);
		for(list<string>::const_iterator j=excludes.begin(); (j!=excludes.end() && match); ++j)
			match=!Regex(*j).match(*i);
		if(match)
			compiler.process_file(base / *i, write_st);
	}
}

void ForEach::exclude(const string &p)
{
	excludes.push_back(p);
}

void ForEach::pattern(const string &p)
{
	patterns.push_back(p);
}

void ForEach::write(const DataFile::Statement &st)
{
	write_st.insert(write_st.end(), st.sub.begin(), st.sub.end());
}
