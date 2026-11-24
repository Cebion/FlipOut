/* $Id: package.cpp 89 2009-05-07 06:08:09Z tdb $

This file is part of builder
Copyright © 2006-2007, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/io/print.h>
#include <msp/strings/lexicalcast.h>
#include <msp/strings/utils.h>
#include "builder.h"
#include "misc.h"
#include "package.h"

using namespace std;
using namespace Msp;

Package::Package(Builder &b, const string &n):
	builder(b),
	name(n),
	conf_done(false),
	use_pkgconfig(true)
{ }

PackageList Package::collect_requires()
{
	PackageList result;
	result.push_back(this);
	for(PackageList::const_iterator i=requires.begin(); i!=requires.end(); ++i)
	{
		PackageList r=(*i)->collect_requires();
		result.splice(result.end(), r);
	}

	result.sort();
	result.unique();

	return result;
}

void Package::configure(const StringMap &opts, unsigned flag)
{
	if(conf_done)
		return;

	if(builder.get_verbose()>=3)
		IO::print("Configuring %s\n", name);

	do_configure(opts, flag);

	requires.sort();
	requires.unique();

	for(PackageList::iterator i=requires.begin(); i!=requires.end(); ++i)
		(*i)->configure(opts, flag&2);

	create_build_info();

	conf_done=true;
}


Package::Loader::Loader(Package &p):
	pkg(p)
{
	add("require", &Loader::require);
}

void Package::Loader::require(const string &n)
{
	Package *req=pkg.builder.get_package(n);
	if(req)
		pkg.requires.push_back(req);
}
