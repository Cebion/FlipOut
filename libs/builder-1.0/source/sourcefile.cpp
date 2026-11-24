/* $Id: sourcefile.cpp 96 2009-09-11 22:48:06Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/fs/utils.h>
#include <msp/io/except.h>
#include <msp/io/print.h>
#include <msp/strings/regex.h>
#include "builder.h"
#include "component.h"
#include "sourcefile.h"
#include "sourcepackage.h"

using namespace std;
using namespace Msp;

SourceFile::SourceFile(Builder &b, const FS::Path &p):
	FileTarget(b, 0, p),
	comp(0)
{ }

SourceFile::SourceFile(Builder &b, const Component &c, const FS::Path &p):
	FileTarget(b, &c.get_package(), p),
	comp(&c)
{ }

void SourceFile::find_depends()
{
	if(!comp)
	{
		deps_ready=true;
		return;
	}

	const SourcePackage &spkg=comp->get_package();
	string relname=FS::relative(name, spkg.get_source()).str();
	DependencyCache &deps_cache=spkg.get_deps_cache();
	bool deps_found=false;
	if(mtime<deps_cache.get_mtime())
	{
		try
		{
			includes=deps_cache.get_deps(relname);
			deps_found=true;
		}
		catch(const KeyError &)
		{ }
	}

	if(!deps_found)
	{
		try
		{
			IO::BufferedFile in(name);

			if(builder.get_verbose()>=4)
				IO::print("Reading includes from %s\n", name);

			Regex r_include("^[ \t]*#include[ \t]+([\"<].*)[\">]");

			string line;
			while(in.getline(line))
				if(RegMatch match=r_include.match(line))
					includes.push_back(match[1].str);

			deps_cache.set_deps(relname, includes);
		}
		catch(const IO::FileNotFound &)
		{
			// XXX WTF?
			return;
		}
	}

	const StringList &incpath=comp->get_build_info().incpath;

	FS::Path dir=FS::dirname(path);
	for(list<string>::iterator i=includes.begin(); i!=includes.end(); ++i)
	{
		Target *hdr=builder.get_header(*i, dir, incpath);
		if(hdr)
			add_depend(hdr);
	}

	deps_ready=true;
}
