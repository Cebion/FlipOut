/* $Id: sourcepackage.cpp 98 2009-09-20 17:34:22Z tdb $

This file is part of builder
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/io/print.h>
#include <msp/strings/lexicalcast.h>
#include <msp/strings/utils.h>
#include "binarypackage.h"
#include "builder.h"
#include "misc.h"
#include "sourcepackage.h"

using namespace std;
using namespace Msp;

namespace {

bool component_sort(const Component &c1, const Component &c2)
{ return c1.get_type()<c2.get_type(); }

}


SourcePackage::SourcePackage(Builder &b, const string &n, const FS::Path &s):
	Package(b, n),
	source(s),
	config(*this),
	deps_cache(*this)
{
	components.push_back(Component(*this, Component::TARBALL, "@src"));
}

FS::Path SourcePackage::get_temp_dir() const
{
	return source/config.get_option("tempdir").value/builder.get_current_arch().get_name()/config.get_option("profile").value;
}

FS::Path SourcePackage::get_out_dir() const
{
	const Architecture &arch=builder.get_current_arch();
	if(arch.is_native())
		return source/config.get_option("outdir").value;
	else
		return source/arch.get_name()/config.get_option("outdir").value;
}

unsigned SourcePackage::get_install_flags()
{
	unsigned flags=0;
	for(ComponentList::iterator i=components.begin(); i!=components.end(); ++i)
		if(i->get_install())
		{
			if(i->get_type()==Component::PROGRAM)
				flags|=BIN;
			else if(i->get_type()==Component::LIBRARY || i->get_type()==Component::MODULE)
				flags|=LIB;
			else if(i->get_type()==Component::HEADERS)
				flags|=INCLUDE;
		}

	return flags;
}

LibMode SourcePackage::get_library_mode() const
{
	const string &mode=config.get_option("staticlibs").value;
	if(mode=="all")
		return ALL_STATIC;
	else if(mode=="local")
		return LOCAL_STATIC;
	else if(mode=="none")
		return DYNAMIC;
	else
		throw Exception("Unknown library mode");
}

void SourcePackage::do_configure(const StringMap &opts, unsigned flag)
{
	init_config();

	StringMap::const_iterator prof=opts.find("profile");
	if(prof!=opts.end() && flag)
		config.select_profile(prof->second);
	else
		config.select_last_profile();

	if(flag && config.update(opts))
	{
		if(builder.get_verbose()>=2)
			IO::print("Configuration of %s changed\n", name);
		if(!builder.get_dry_run())
			config.save();
	}

	config.finish();

	for(ConditionList::iterator i=conditions.begin(); i!=conditions.end(); ++i)
		if(i->eval())
		{
			const StringList &reqs=i->get_requires();
			for(StringList::const_iterator j=reqs.begin(); j!=reqs.end(); ++j)
				if(Package *pkg=builder.get_package(*j))
					requires.push_back(pkg);
		}

	base_reqs=requires;

	for(ComponentList::iterator i=components.begin(); i!=components.end(); ++i)
	{
		const PackageList &reqs=i->get_requires();
		requires.insert(requires.end(), reqs.begin(), reqs.end());
	}

	for(PackageList::iterator i=requires.begin(); i!=requires.end(); ++i)
	{
		BinaryPackage *bpkg=dynamic_cast<BinaryPackage *>(*i);
		if(bpkg && bpkg->get_need_path())
			bpkg->set_path(config.get_option(bpkg->get_name()+"_path").value);
	}

	deps_cache.load();
}

void SourcePackage::init_config()
{
	config.add_option("profile",    "default", "Configuration profile");
	config.add_option("tempdir",    "temp",    "Directory for storing temporary files");
	config.add_option("outdir",     ".",       "Directory to put build results in");
	config.add_option("optimize",   "0",       "Apply compiler optimizations");
	config.add_option("strip",      "0",       "Strip symbols from programs");
	config.add_option("debug",      "0",       "Produce debugging symbols");
	config.add_option("cpu",        "none",    "CPU type to optimize for");
	config.add_option("staticlibs", "local",   "Use static libraries");

	for(FeatureList::iterator i=features.begin(); i!=features.end(); ++i)
		config.add_option("with_"+i->name, "0", i->descr);

	for(PackageList::const_iterator i=requires.begin(); i!=requires.end(); ++i)
	{
		BinaryPackage *bpkg=dynamic_cast<BinaryPackage *>(*i);
		if(bpkg && bpkg->get_need_path())
			config.add_option(bpkg->get_name()+"_path", "/usr", "Path for "+bpkg->get_name());
	}
}

void SourcePackage::create_build_info()
{
	for(PackageList::iterator i=base_reqs.begin(); i!=base_reqs.end(); ++i)
	{
		const BuildInfo &ebi=(*i)->get_exported_binfo();
		build_info.add(ebi);

		export_binfo.cflags.insert(export_binfo.cflags.end(), ebi.cflags.begin(), ebi.cflags.end());
		export_binfo.incpath.insert(export_binfo.incpath.end(), ebi.incpath.begin(), ebi.incpath.end());
		export_binfo.defines.insert(export_binfo.defines.end(), ebi.defines.begin(), ebi.defines.end());
	}

	// XXX Currently, a package-specific settings will override cmdline.  This might or might not be desirable.
	const StringList &warnings=builder.get_warnings();
	build_info.warnings.insert(build_info.warnings.begin(), warnings.begin(), warnings.end());

	unsigned flags=get_install_flags();

	build_info.incpath.push_back((builder.get_prefix()/"include").str());
	build_info.libpath.push_back((builder.get_prefix()/"lib").str());

	if(flags&INCLUDE)
		export_binfo.incpath.push_back((builder.get_prefix()/"include").str());
	if(flags&LIB)
		export_binfo.libpath.push_back((builder.get_prefix()/"lib").str());

	string optimize=config.get_option("optimize").value;
	if(lexical_cast<unsigned>(optimize))
	{
		build_info.cflags.push_back("-O"+optimize);
		build_info.ldflags.push_back("-O"+optimize);
		string cpu=config.get_option("cpu").value;
		if(cpu!="none")
			build_info.cflags.push_back("-march="+cpu);
	}

	if(lexical_cast<bool>(config.get_option("debug").value))
	{
		build_info.cflags.push_back("-ggdb");
		build_info.defines.push_back("DEBUG");
	}

	for(FeatureList::iterator i=features.begin(); i!=features.end(); ++i)
		if(lexical_cast<bool>(config.get_option("with_"+i->name).value))
			build_info.cflags.push_back("-DWITH_"+toupper(i->name));

	for(ConditionList::iterator i=conditions.begin(); i!=conditions.end(); ++i)
		if(i->eval())
			build_info.add(i->get_build_info());

	build_info.unique();

	for(list<Component>::iterator i=components.begin(); i!=components.end(); ++i)
	{
		i->create_build_info();
		if(i->get_type()==Component::LIBRARY)
			export_binfo.libs.push_back(i->get_name());
	}

	export_binfo.unique();
}


SourcePackage::Loader::Loader(Package &p):
	Package::Loader(p)
{
	add("version",     &SourcePackage::version);
	add("description", &SourcePackage::description);
	add("build_info",  &Loader::build_info);
	add("feature",     &Loader::feature);
	add("if",          &Loader::condition);
	add("program",     &Loader::component<Component::PROGRAM>);
	add("library",     &Loader::component<Component::LIBRARY>);
	add("module",      &Loader::component<Component::MODULE>);
	add("headers",     &Loader::component<Component::HEADERS>);
	add("install",     &Loader::component<Component::INSTALL>);
	add("datafile",    &Loader::component<Component::DATAFILE>);
	add("tarball",     &Loader::tarball);
	add("tar_file",    &Loader::tar_file);
}

void SourcePackage::Loader::finish()
{
	SourcePackage &spkg=static_cast<SourcePackage &>(pkg);
	spkg.components.sort(component_sort);
}

void SourcePackage::Loader::feature(const string &n, const string &d)
{
	static_cast<SourcePackage &>(pkg).features.push_back(Feature(n, d));
}

void SourcePackage::Loader::condition(const string &c)
{
	SourcePackage &spkg=static_cast<SourcePackage &>(pkg);
	Condition cond(spkg, c);
	load_sub(cond);
	spkg.conditions.push_back(cond);
}

template<Component::Type t>
void SourcePackage::Loader::component(const string &n)
{
	SourcePackage &spkg=static_cast<SourcePackage &>(pkg);
	Component comp(spkg, t, n);
	load_sub(comp);
	spkg.components.push_back(comp);
}

void SourcePackage::Loader::build_info()
{
	load_sub(static_cast<SourcePackage &>(pkg).build_info);
}

void SourcePackage::Loader::tarball(const string &n)
{
	SourcePackage &spkg=static_cast<SourcePackage &>(pkg);
	if(n=="@src")
	{
		for(ComponentList::iterator i=spkg.components.begin(); i!=spkg.components.end(); ++i)
			if(i->get_type()==Component::TARBALL && i->get_name()==n)
				load_sub(*i);
	}
	else
	{
		Component trbl(spkg, Component::TARBALL, n);
		load_sub(trbl);
	}
}

void SourcePackage::Loader::tar_file(const string &f)
{
	IO::print("%s: Note: tar_file is deprecated\n", get_source());
	SourcePackage &spkg=static_cast<SourcePackage &>(pkg);
	for(ComponentList::iterator i=spkg.components.begin(); i!=spkg.components.end(); ++i)
		if(i->get_type()==Component::TARBALL && i->get_name()=="@src")
			const_cast<PathList &>(i->get_sources()).push_back(spkg.source/f);
}
