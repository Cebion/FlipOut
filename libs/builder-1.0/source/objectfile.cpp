/* $Id: objectfile.cpp 87 2009-03-17 11:18:42Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <algorithm>
#include <msp/fs/utils.h>
#include "builder.h"
#include "compile.h"
#include "component.h"
#include "install.h"
#include "objectfile.h"
#include "sourcefile.h"
#include "sourcepackage.h"

using namespace std;
using namespace Msp;

ObjectFile::ObjectFile(Builder &b, const Component &c, SourceFile &s):
	FileTarget(b, &c.get_package(), generate_target_path(c, FS::basename(s.get_path()))),
	comp(c),
	source(s)	
{
	buildable=true;
	add_depend(&source);
}

void ObjectFile::find_depends()
{
	for(TargetList::iterator i=new_deps.begin(); i!=new_deps.end();)
	{
		Target *tgt=*i;
		if(tgt->get_depends_ready())
		{
			i=new_deps.erase(i);
			find_depends(tgt);
		}
		else
			++i;
	}

	deps_ready=new_deps.empty();
}


void ObjectFile::find_depends(Target *tgt)
{
	SourceFile *src=dynamic_cast<SourceFile *>(tgt);
	FileTarget *file=src;
	if(!src)
	{
		if(Install *inst=dynamic_cast<Install *>(tgt))
		{
			file=inst;
			src=dynamic_cast<SourceFile *>(&inst->get_source());
		}
	}
	if(!src)
		return;

	FS::Path spath=FS::dirname(file->get_path());
	const StringList &incpath=comp.get_build_info().incpath;

	const list<string> &includes=src->get_includes();
	for(list<string>::const_iterator i=includes.begin(); i!=includes.end(); ++i)
	{
		Target *hdr2=builder.get_header(*i, spath, incpath);
		if(hdr2 && find(depends.begin(), depends.end(), hdr2)==depends.end())
			add_depend(hdr2);
	}
}

void ObjectFile::add_depend(Target *tgt)
{
	Target::add_depend(tgt);
	new_deps.push_back(tgt);
}

Action *ObjectFile::create_action()
{
	return new Compile(builder, *this);
}

FS::Path ObjectFile::generate_target_path(const Component &comp, const string &src)
{
	const SourcePackage &pkg=comp.get_package();
	return pkg.get_temp_dir()/comp.get_name()/(FS::basepart(src)+".o");
}
