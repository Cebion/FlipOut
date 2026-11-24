/* $Id: binary.cpp 85 2009-02-21 00:45:50Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/fs/utils.h>
#include <msp/strings/formatter.h>
#include "binary.h"
#include "builder.h"
#include "component.h"
#include "install.h"
#include "link.h"
#include "objectfile.h"
#include "package.h"
#include "sharedlibrary.h"
#include "staticlibrary.h"

using namespace std;
using namespace Msp;

Binary::Binary(Builder &b, const Component &c, const list<ObjectFile *> &objs):
	FileTarget(b, &c.get_package(), generate_target_path(c)),
	comp(c)
{
	buildable=true;
	for(list<ObjectFile *>::const_iterator i=objs.begin(); i!=objs.end(); ++i)
		add_depend(*i);
}

void Binary::find_depends()
{
	LibMode libmode=comp.get_package().get_library_mode();
	if(dynamic_cast<SharedLibrary *>(this))
		libmode=DYNAMIC;

	list<const Component *> queue;
	list<Target *> dep_libs;
	queue.push_back(&comp);
	while(!queue.empty())
	{
		const Component *c=queue.front();
		queue.erase(queue.begin());

		const StringList &libpath=c->get_build_info().libpath;

		const list<string> &libs=c->get_build_info().libs;
		for(StringList::const_iterator i=libs.begin(); i!=libs.end(); ++i)
		{
			Target *lib=builder.get_library(*i, libpath, libmode);
			if(lib)
			{
				dep_libs.push_back(lib);

				if(Install *inst=dynamic_cast<Install *>(lib))
					lib=&inst->get_source();
				if(StaticLibrary *stlib=dynamic_cast<StaticLibrary *>(lib))
					queue.push_back(&stlib->get_component());
			}
			else
				builder.problem(comp.get_package().get_name(), format("Couldn't find library %s for %s", *i, FS::basename(name)));
		}
	}

	/* Add only the last occurrence of each library to the actual dependencies.
	This ensures that static library ordering is correct. */
	for(list<Target *>::iterator i=dep_libs.begin(); i!=dep_libs.end(); ++i)
	{
		bool last=true;
		for(list<Target *>::iterator j=i; (last && j!=dep_libs.end()); ++j)
			last=(j==i || *j!=*i);
		if(last)
			add_depend(*i);
	}

	deps_ready=true;
}

Action *Binary::create_action()
{
	return new Link(builder, *this);
}

FS::Path Binary::generate_target_path(const Component &c)
{
	const SourcePackage &pkg=c.get_package();
	string prefix, suffix;
	const string &arch=pkg.get_builder().get_current_arch().get_name();

	if(c.get_type()==Component::LIBRARY)
	{
		prefix="lib";
		if(arch=="win32")
			suffix=".dll";
		else
			suffix=".so";
	}
	else if(c.get_type()==Component::MODULE)
		suffix=".m";
	else if(c.get_type()==Component::PROGRAM)
	{
		if(arch=="win32")
			suffix=".exe";
	}

	return pkg.get_out_dir()/(prefix+c.get_name()+suffix);
}
