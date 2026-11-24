/* $Id$

This file is part of builder
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/fs/stat.h>
#include <msp/io/except.h>
#include <msp/io/file.h>
#include <msp/io/print.h>
#include <msp/strings/utils.h>
#include "builder.h"
#include "dependencycache.h"
#include "sourcepackage.h"

using namespace std;
using namespace Msp;

DependencyCache::DependencyCache(SourcePackage &p):
	package(p),
	changed(false)
{ }

void DependencyCache::set_deps(const string &tgt, const StringList &d)
{
	deps[tgt]=d;
	changed=true;
}

const StringList &DependencyCache::get_deps(const string &tgt) const
{
	DepsMap::const_iterator i=deps.find(tgt);
	if(i==deps.end())
		throw KeyError("Unknown dependencies", tgt);

	return i->second;
}

void DependencyCache::save() const
{
	if(deps.empty() || !changed || package.get_builder().get_dry_run())
		return;

	IO::BufferedFile out((package.get_source()/".deps").str(), IO::M_WRITE);

	for(DepsMap::const_iterator i=deps.begin(); i!=deps.end(); ++i)
	{
		IO::print(out, i->first);
		for(StringList::const_iterator j=i->second.begin(); j!=i->second.end(); ++j)
			IO::print(out, "|%s", *j);
		IO::print(out, "\n");
	}
}

void DependencyCache::load()
{
	string fn=(package.get_source()/".deps").str();

	try
	{
		IO::BufferedFile in(fn);

		string line;
		while(in.getline(line))
		{
			vector<string> parts=split(line, '|');
			deps[parts[0]]=StringList(parts.begin()+1, parts.end());
		}

		mtime=Time::TimeStamp::from_unixtime(FS::stat(fn).st_mtime);
	}
	catch(const IO::FileNotFound &)
	{ }
}
