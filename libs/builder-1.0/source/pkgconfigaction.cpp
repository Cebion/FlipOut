/* $Id: pkgconfigaction.cpp 101 2009-09-21 16:19:49Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/fs/utils.h>
#include <msp/io/file.h>
#include <msp/io/print.h>
#include "package.h"
#include "pkgconfig.h"
#include "pkgconfigaction.h"

using namespace std;
using namespace Msp;

PkgConfigAction::PkgConfigAction(Builder &b, const PkgConfig &p):
	Action(b)
{
	const SourcePackage &spkg=*static_cast<const SourcePackage *>(p.get_package());

	announce(spkg.get_name(), "PC", basename(p.get_path()));

	IO::BufferedFile out(p.get_path().str(), IO::M_WRITE);
	// Prefix is already included in the various paths
	//IO::print(out, "prefix=%s\n", pkg.get_prefix());
	IO::print(out, "source=%s\n\n", spkg.get_source());

	IO::print(out, "Name: %s\n", spkg.get_name());
	IO::print(out, "Description: %s\n", spkg.get_description());
	IO::print(out, "Version: %s\n", spkg.get_version());

	IO::print(out, "Requires:");
	const PackageList &reqs=spkg.get_requires();
	for(PackageList::const_iterator i=reqs.begin(); i!=reqs.end(); ++i)
		if((*i)->get_use_pkgconfig())
			IO::print(out, " %s", (*i)->get_name());
	out.put('\n');

	const BuildInfo &binfo=spkg.get_exported_binfo();
	IO::print(out, "Libs:");
	for(StringList::const_iterator i=binfo.libpath.begin(); i!=binfo.libpath.end(); ++i)
		IO::print(out, " -L%s", *i);
	for(StringList::const_iterator i=binfo.libs.begin(); i!=binfo.libs.end(); ++i)
		IO::print(out, " -l%s", *i);
	for(StringList::const_iterator i=binfo.ldflags.begin(); i!=binfo.ldflags.end(); ++i)
		IO::print(out, " %s", *i);
	out.put('\n');

	IO::print(out, "Cflags:");
	for(StringList::const_iterator i=binfo.incpath.begin(); i!=binfo.incpath.end(); ++i)
		IO::print(out, " -I%s", *i);
	for(StringList::const_iterator i=binfo.defines.begin(); i!=binfo.defines.end(); ++i)
		IO::print(out, " -D%s", *i);
	for(StringList::const_iterator i=binfo.cflags.begin(); i!=binfo.cflags.end(); ++i)
		IO::print(out, " %s", *i);
	out.put('\n');
}

int PkgConfigAction::check()
{
	signal_done.emit();
	return 0;
}
