/* $Id: link.cpp 101 2009-09-21 16:19:49Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/fs/dir.h>
#include <msp/fs/utils.h>
#include "builder.h"
#include "component.h"
#include "executable.h"
#include "install.h"
#include "link.h"
#include "objectfile.h"
#include "package.h"
#include "sharedlibrary.h"
#include "staticlibrary.h"
#include "systemlibrary.h"

using namespace std;
using namespace Msp;

Link::Link(Builder &b, const Binary &bin):
	ExternalAction(b)
{
	const Component &comp=bin.get_component();

	work_dir=comp.get_package().get_source();

	//XXX Determine whether to use g++ or gcc
	string tool="LXX";
	argv.push_back(builder.get_current_arch().get_tool(tool));

	if(comp.get_type()==Component::LIBRARY || comp.get_type()==Component::MODULE)
		argv.push_back("-shared");
	else if(comp.get_package().get_library_mode()==ALL_STATIC)
		argv.push_back("-static");

	const BuildInfo &binfo=comp.get_build_info();
	for(list<string>::const_iterator i=binfo.ldflags.begin(); i!=binfo.ldflags.end(); ++i)
		argv.push_back(*i);
	for(list<string>::const_iterator i=binfo.libpath.begin(); i!=binfo.libpath.end(); ++i)
		argv.push_back("-L"+*i);

	argv.push_back("-o");
	argv.push_back(relative(bin.get_path(), work_dir).str());
	const TargetList &deps=bin.get_depends();
	for(TargetList::const_iterator i=deps.begin(); i!=deps.end(); ++i)
	{
		Target *tgt=*i;
		if(Install *inst=dynamic_cast<Install *>(tgt))
			tgt=&inst->get_source();

		if(ObjectFile *obj=dynamic_cast<ObjectFile *>(tgt))
			argv.push_back(relative(obj->get_path(), work_dir).str());
		else if(StaticLibrary *stlib=dynamic_cast<StaticLibrary *>(tgt))
			argv.push_back(stlib->get_path().str());
		else if(Library *lib=dynamic_cast<Library *>(tgt))
			argv.push_back("-l"+lib->get_libname());
	}

	FS::Path binpath=bin.get_path();
	if(!builder.get_dry_run())
		FS::mkpath(FS::dirname(binpath), 0755);

	announce(comp.get_package().get_name(), tool, relative(binpath, work_dir).str().substr(2));

	launch();
}
