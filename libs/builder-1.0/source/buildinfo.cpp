/* $Id: buildinfo.cpp 84 2009-02-19 16:43:37Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <algorithm>
#include "buildinfo.h"

using namespace std;
using namespace Msp;

namespace {

/** Removes any duplicate entries from a list, leaving only the first one.  The
order of other elements is preserved.  O(n²) efficiency. */
void unique(StringList &l)
{
	for(StringList::iterator i=l.begin(); i!=l.end(); ++i)
		for(StringList::iterator j=i; j!=l.end();)
		{
			if(j!=i && *j==*i)
				j=l.erase(j);
			else
				++j;
		}
}

}


void BuildInfo::add(const BuildInfo &bi)
{
	cflags.insert(cflags.end(), bi.cflags.begin(), bi.cflags.end());
	defines.insert(defines.end(), bi.defines.begin(), bi.defines.end());
	incpath.insert(incpath.end(), bi.incpath.begin(), bi.incpath.end());
	ldflags.insert(ldflags.end(), bi.ldflags.begin(), bi.ldflags.end());
	libpath.insert(libpath.end(), bi.libpath.begin(), bi.libpath.end());
	libs.insert(libs.end(), bi.libs.begin(), bi.libs.end());
	warnings.insert(warnings.end(), bi.warnings.begin(), bi.warnings.end());
}

void BuildInfo::unique()
{
	::unique(cflags);
	::unique(defines);
	::unique(incpath);
	::unique(ldflags);
	::unique(libpath);
	::unique(libs);

	for(StringList::iterator i=warnings.begin(); i!=warnings.end(); ++i)
	{
		bool flag=i->compare(0, 3, "no-");

		string warn=(flag ? *i : i->substr(3));
		string no_warn="no-"+warn;

		for(StringList::iterator j=i; j!=warnings.end();)
		{
			if(j!=i && (*j==warn || *j==no_warn))
			{
				flag=(*j==warn);
				j=warnings.erase(j);
			}
			else
				++j;
		}

		*i=(flag ? warn : no_warn);
	}
}


BuildInfo::Loader::Loader(BuildInfo &bi):
	binfo(bi)
{
	add("cflag",   &Loader::cflag);
	add("incpath", &Loader::incpath);
	add("define",  &Loader::define);
	add("ldflag",  &Loader::ldflag);
	add("libpath", &Loader::libpath);
	add("library", &Loader::library);
	add("warning", &Loader::warning);
}

void BuildInfo::Loader::cflag(const std::string &s)
{
	binfo.cflags.push_back(s);
}

void BuildInfo::Loader::incpath(const std::string &s)
{
	binfo.incpath.push_back(s);
}

void BuildInfo::Loader::define(const std::string &s)
{
	binfo.defines.push_back(s);
}

void BuildInfo::Loader::ldflag(const std::string &s)
{
	binfo.ldflags.push_back(s);
}

void BuildInfo::Loader::libpath(const std::string &s)
{
	binfo.libpath.push_back(s);
}

void BuildInfo::Loader::library(const std::string &s)
{
	binfo.libs.push_back(s);
}

void BuildInfo::Loader::warning(const std::string &s)
{
	binfo.warnings.push_back(s);
}
