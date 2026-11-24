/* $Id: archive.cpp 101 2009-09-21 16:19:49Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/fs/dir.h>
#include <msp/fs/stat.h>
#include <msp/fs/utils.h>
#include "archive.h"
#include "builder.h"
#include "component.h"
#include "objectfile.h"
#include "sourcepackage.h"
#include "staticlibrary.h"

using namespace std;
using namespace Msp;

Archive::Archive(Builder &b, const StaticLibrary &lib):
	ExternalAction(b)
{
	const Component &comp=lib.get_component();

	work_dir=comp.get_package().get_source();

	std::string tool="AR";
	argv.push_back(builder.get_current_arch().get_tool(tool));
	argv.push_back("rc");

	argv.push_back(relative(lib.get_path(), work_dir).str());
	const TargetList &deps=lib.get_depends();
	for(TargetList::const_iterator i=deps.begin(); i!=deps.end(); ++i)
		if(ObjectFile *obj=dynamic_cast<ObjectFile *>(*i))
			argv.push_back(relative(obj->get_path(), work_dir).str());

	FS::Path lpath=lib.get_path();
	if(!builder.get_dry_run())
	{
		FS::mkpath(FS::dirname(lpath), 0755);
		if(FS::exists(lib.get_path()))
			FS::unlink(lib.get_path());
	}

	announce(comp.get_package().get_name(), tool, relative(lpath, work_dir).str().substr(2));

	launch();
}
