/* $Id: component.cpp 100 2009-09-21 09:54:09Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/core/except.h>
#include <msp/fs/dir.h>
#include <msp/fs/stat.h>
#include <msp/fs/utils.h>
#include <msp/io/print.h>
#include <msp/strings/lexicalcast.h>
#include "builder.h"
#include "component.h"
#include "datafile.h"
#include "executable.h"
#include "file.h"
#include "header.h"
#include "install.h"
#include "objectfile.h"
#include "sharedlibrary.h"
#include "sourcepackage.h"
#include "staticlibrary.h"
#include "tarball.h"
#include "target.h"

using namespace std;
using namespace Msp;

Component::Component(SourcePackage &p, Type t, const string &n):
	pkg(p),
	type(t),
	name(n),
	install(false),
	module_host(0),
	modular(false),
	deflt(true)
{ }

void Component::create_build_info()
{
	build_info.add(pkg.get_build_info());

	for(PackageList::iterator i=requires.begin(); i!=requires.end(); ++i)
		build_info.add((*i)->get_exported_binfo());

	for(StringList::iterator i=build_info.incpath.begin(); i!=build_info.incpath.end(); ++i)
		*i=(pkg.get_source() / *i).str();
	for(StringList::iterator i=build_info.libpath.begin(); i!=build_info.libpath.end(); ++i)
		*i=(pkg.get_source() / *i).str();

	if(pkg.get_library_mode()!=DYNAMIC)
	{
		// XXX This may pull in some unnecessary libpaths too.  More thought required.
		PackageList reqs=pkg.collect_requires();
		for(PackageList::iterator i=reqs.begin(); i!=reqs.end(); ++i)
		{
			const BuildInfo &ebi=(*i)->get_exported_binfo();
			build_info.libpath.insert(build_info.libpath.end(), ebi.libpath.begin(), ebi.libpath.end());
		}
	}

	if(type==PROGRAM)
	{
		string strip=pkg.get_config().get_option("strip").value;
		if(lexical_cast<bool>(strip))
			build_info.ldflags.push_back("-s");
	}

	if(modular)
	{
		build_info.ldflags.push_back("-rdynamic");
		build_info.libs.push_back("dl");
	}
	else if(module_host)
	{
		const PathList &host_src=module_host->get_sources();
		for(PathList::const_iterator i=host_src.begin(); i!=host_src.end(); ++i)
			build_info.incpath.push_back(i->str());
	}

	build_info.unique();
}

void Component::create_targets() const
{
	Builder &builder=pkg.get_builder();
	Target *world=builder.get_target("world");
	Target *def_tgt=builder.get_target("default");

	PathList files=collect_source_files();
	list<FileTarget *> inst_list;

	string inst_loc;
	if(type==TARBALL)
	{
		string tarname=name;
		if(name=="@src")
			tarname=pkg.get_name()+"-"+pkg.get_version();
		TarBall *result=new TarBall(builder, pkg, tarname);

		if(name=="@src")
		{
			const TargetMap &targets=builder.get_targets();
			for(TargetMap::const_iterator i=targets.begin(); i!=targets.end(); ++i)
				if(i->second->get_package()==&pkg && !i->second->is_buildable())
					result->add_depend(i->second);
			files.push_back(pkg.get_source()/"Build");
		}

		for(PathList::const_iterator i=files.begin(); i!=files.end(); ++i)
		{
			FileTarget *ft;
			if(Target *tgt=builder.get_target(i->str()))
				ft=dynamic_cast<FileTarget *>(tgt);
			else
				ft=new File(builder, *i);
			result->add_depend(ft);
		}

		Target *tarbls_tgt=builder.get_target("tarballs");
		tarbls_tgt->add_depend(result);

		return;
	}
	else if(type==INSTALL)
	{
		inst_loc=name;
		for(PathList::const_iterator i=files.begin(); i!=files.end(); ++i)
		{
			FileTarget *ft;
			if(Target *tgt=builder.get_target(i->str()))
				ft=dynamic_cast<FileTarget *>(tgt);
			else
				ft=new File(builder, pkg, *i);
			inst_list.push_back(ft);
		}
	}
	else if(type==DATAFILE)
	{
		File *source;
		if(Target *tgt=builder.get_target(files.front().str()))
			source=dynamic_cast<File *>(tgt);
		else
			source=new File(builder, pkg, files.front());
		::DataFile *result=new ::DataFile(builder, *this, *source);

		if(&pkg==builder.get_main_package() && deflt)
			def_tgt->add_depend(result);
		else
			world->add_depend(result);
		if(install)
			inst_list.push_back(result);
	}
	else
	{
		for(PathList::const_iterator i=files.begin(); i!=files.end(); ++i)
		{
			string ext=FS::extpart(FS::basename(*i));
			if(ext==".h")
			{
				FileTarget *hdr=dynamic_cast<FileTarget *>(builder.get_target(i->str()));
				if(!hdr)
					hdr=new Header(builder, *this, i->str());

				// Install headers if requested
				if(type==HEADERS && install)
					inst_list.push_back(hdr);
			}
		}
	}

	if(type==PROGRAM || type==LIBRARY || type==MODULE)
	{
		list<ObjectFile *> objs;
		for(PathList::const_iterator i=files.begin(); i!=files.end(); ++i)
		{
			string ext=FS::extpart(FS::basename(*i));
			if((ext==".cpp" || ext==".cc" || ext==".c"))
			{
				SourceFile *src=new SourceFile(builder, *this, i->str());
				ObjectFile *obj=new ObjectFile(builder, *this, *src);
				objs.push_back(obj);
			}
		}

		list<FileTarget *> results;
		if(type==LIBRARY)
		{
			results.push_back(new SharedLibrary(builder, *this, objs));
			results.push_back(new StaticLibrary(builder, *this, objs));
		}
		else
			results.push_back(new Executable(builder, *this, objs));

		for(list<FileTarget *>::const_iterator i=results.begin(); i!=results.end(); ++i)
		{
			if(&pkg==builder.get_main_package() && deflt)
				def_tgt->add_depend(*i);
			else
				world->add_depend(*i);
			if(install)
				inst_list.push_back(*i);
		}
	}

	Target *inst_tgt=builder.get_target("install");
	for(list<FileTarget *>::const_iterator i=inst_list.begin(); i!=inst_list.end(); ++i)
		inst_tgt->add_depend(new Install(builder, pkg, **i, inst_loc));
}

PathList Component::collect_source_files() const
{
	PathList files;
	for(PathList::const_iterator i=sources.begin(); i!=sources.end(); ++i)
	{
		if(FS::is_dir(*i))
		{
			list<string> sfiles=list_files(*i);
			for(list<string>::iterator j=sfiles.begin(); j!=sfiles.end(); ++j)
				files.push_back(*i / *j);
		}
		else
			files.push_back(*i);
	}

	return files;
}


Component::Loader::Loader(Component &c):
	comp(c)
{
	add("source",          &Loader::source);
	add("install",         &Component::install);
	add("install_headers", &Loader::install_headers);
	add("build_info",      &Loader::build_info);
	add("require",         &Loader::require);
	add("modular",         &Loader::modular);
	add("host",            &Loader::host);
	add("default",         &Component::deflt);
}

void Component::Loader::finish()
{
	if(!inst_hdr.empty())
	{
		Component hdrcomp(comp.pkg, HEADERS, inst_hdr);
		hdrcomp.sources=comp.sources;
		hdrcomp.install=true;
		const_cast<ComponentList &>(comp.pkg.get_components()).push_back(hdrcomp);
	}
}

void Component::Loader::source(const string &s)
{
	comp.sources.push_back(comp.pkg.get_source()/s);
}

void Component::Loader::require(const string &n)
{
	Package *req=comp.pkg.get_builder().get_package(n);
	if(req)
		comp.requires.push_back(req);
}

void Component::Loader::modular()
{
	if(comp.type!=PROGRAM)
		throw Exception("Only programs can be modular");
	comp.modular=true;
}

void Component::Loader::host(const string &n)
{
	const ComponentList &comps=comp.pkg.get_components();
	for(ComponentList::const_iterator i=comps.begin(); i!=comps.end(); ++i)
		if(i->get_name()==n)
		{
			if(i->get_type()!=PROGRAM || !i->is_modular())
				throw Exception("Module host must be a modular program");
			comp.module_host=&*i;
			return;
		}

	throw KeyError("Unknown component", n);
}

void Component::Loader::install_headers(const string &p)
{
	IO::print("%s: Note: install_headers is deprecated\n", get_source());
	if(comp.type==HEADERS)
	{
		comp.name=p;
		comp.install=true;
	}
	else
		inst_hdr=p;
}

void Component::Loader::build_info()
{
	load_sub(comp.build_info);
}
