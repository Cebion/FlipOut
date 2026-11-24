/* $Id: compile.cpp 101 2009-09-21 16:19:49Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/fs/dir.h>
#include <msp/fs/utils.h>
#include "builder.h"
#include "buildinfo.h"
#include "compile.h"
#include "component.h"
#include "objectfile.h"
#include "sourcefile.h"
#include "sourcepackage.h"

using namespace std;
using namespace Msp;

Compile::Compile(Builder &b, const ObjectFile &obj):
	ExternalAction(b)
{
	const Component &comp=obj.get_component();

	work_dir=comp.get_package().get_source();

	FS::Path spath=obj.get_source().get_path();

	string ext=FS::extpart(spath.str());
	const char *tool=0;
	if(ext==".cpp" || ext==".cc")
		tool="CXX";
	else
		tool="CC";

	argv.push_back(builder.get_current_arch().get_tool(tool));
	argv.push_back("-c");

	const BuildInfo &binfo=comp.get_build_info();
	for(list<string>::const_iterator i=binfo.warnings.begin(); i!=binfo.warnings.end(); ++i)
		argv.push_back("-W"+*i);
	for(list<string>::const_iterator i=binfo.cflags.begin(); i!=binfo.cflags.end(); ++i)
		argv.push_back(*i);
	for(list<string>::const_iterator i=binfo.incpath.begin(); i!=binfo.incpath.end(); ++i)
		argv.push_back("-I"+*i);
	for(list<string>::const_iterator i=binfo.defines.begin(); i!=binfo.defines.end(); ++i)
		argv.push_back("-D"+*i);

	FS::Path opath=obj.get_path();
	argv.push_back("-o");
	argv.push_back(relative(opath, work_dir).str());
	argv.push_back(relative(spath, work_dir).str());

	if(!builder.get_dry_run())
		FS::mkpath(FS::dirname(opath), 0755);

	announce(comp.get_package().get_name(), tool, basename(opath));

	launch();
}
