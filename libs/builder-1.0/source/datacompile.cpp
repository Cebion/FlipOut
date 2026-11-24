/* $Id: datacompile.cpp 98 2009-09-20 17:34:22Z tdb $

This file is part of builder
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/fs/dir.h>
#include <msp/fs/utils.h>
#include "builder.h"
#include "component.h"
#include "datacompile.h"
#include "datafile.h"
#include "file.h"
#include "sourcepackage.h"

using namespace Msp;

DataCompile::DataCompile(Builder &b, ::DataFile &dfile):
	ExternalAction(b)
{
	const Component &comp=dfile.get_component();

	work_dir=comp.get_package().get_source();

	argv.push_back("mspdatatool");
	argv.push_back("-c");
	argv.push_back("-b");

	FS::Path opath=dfile.get_path();
	argv.push_back("-o");
	argv.push_back(relative(opath, work_dir).str());

	FS::Path spath=dfile.get_source().get_path();
	argv.push_back(relative(spath, work_dir).str());

	if(!builder.get_dry_run())
		FS::mkpath(FS::dirname(opath), 0755);

	announce(comp.get_package().get_name(), "DATA", relative(opath, work_dir).str());

	launch();
}
