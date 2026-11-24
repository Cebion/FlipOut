/* $Id: builder.cpp 103 2009-09-21 18:57:29Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <set>
#include <cstdlib>
#include <sys/utsname.h>
#include <msp/core/except.h>
#include <msp/core/getopt.h>
#include <msp/datafile/parser.h>
#include <msp/fs/dir.h>
#include <msp/fs/stat.h>
#include <msp/fs/utils.h>
#include <msp/io/buffered.h>
#include <msp/io/except.h>
#include <msp/io/file.h>
#include <msp/io/print.h>
#include <msp/strings/formatter.h>
#include <msp/strings/regex.h>
#include <msp/strings/utils.h>
#include <msp/time/units.h>
#include <msp/time/utils.h>
#include "action.h"
#include "analyzer.h"
#include "binarypackage.h"
#include "builder.h"
#include "header.h"
#include "install.h"
#include "misc.h"
#include "package.h"
#include "pkgconfig.h"
#include "sharedlibrary.h"
#include "sourcepackage.h"
#include "systemlibrary.h"
#include "unlink.h"
#include "virtualtarget.h"

using namespace std;
using namespace Msp;

namespace {

void update_hash(string &hash, const string &value)
{
	for(unsigned i=0; i<value.size(); ++i)
		hash[i%hash.size()]^=value[i];
}

}


Builder::Builder(int argc, char **argv):
	main_pkg(0),
	analyzer(0),
	build(false),
	clean(0),
	dry_run(false),
	help(false),
	verbose(1),
	show_progress(false),
	build_file("Build"),
	jobs(1),
	conf_all(false),
	conf_only(false),
	build_all(false),
	create_makefile(false)
{
	string analyze_mode;
	string work_dir;
	bool full_paths=false;
	unsigned max_depth=5;
	StringList cmdline_warn;
	string prfx;
	string arch;

	GetOpt getopt;
	getopt.add_option('a', "analyze",    analyze_mode,  GetOpt::REQUIRED_ARG).set_help("Perform analysis.  MODE can be deps, alldeps or rebuild.", "MODE");
	getopt.add_option('b', "build",      build,         GetOpt::NO_ARG).set_help("Perform build even if doing analysis.");
	getopt.add_option('c', "clean",      clean,         GetOpt::NO_ARG).set_help("Clean buildable targets.");
	getopt.add_option('f', "file",       build_file,    GetOpt::REQUIRED_ARG).set_help("Read info from FILE instead of Build.", "FILE");
	getopt.add_option('h', "help",       help,          GetOpt::NO_ARG).set_help("Print this message.");
	getopt.add_option('j', "jobs",       jobs,          GetOpt::REQUIRED_ARG).set_help("Run NUM commands at once, whenever possible.", "NUM");
	getopt.add_option('n', "dry-run",    dry_run,       GetOpt::NO_ARG).set_help("Don't actually do anything, only show what would be done.");
	getopt.add_option('v', "verbose",    verbose,       GetOpt::NO_ARG).set_help("Print more information about what's going on.");
	getopt.add_option('A', "conf-all",   conf_all,      GetOpt::NO_ARG).set_help("Apply configuration to all packages.");
	getopt.add_option('B', "build-all",  build_all,     GetOpt::NO_ARG).set_help("Build all targets unconditionally.");
	getopt.add_option('C', "chdir",      work_dir,      GetOpt::REQUIRED_ARG).set_help("Change to DIR before doing anything else.", "DIR");
	getopt.add_option('P', "progress",   show_progress, GetOpt::NO_ARG).set_help("Display progress while building.");
	getopt.add_option('W', "what-if",    what_if,       GetOpt::REQUIRED_ARG).set_help("Pretend that FILE has changed.", "FILE");
	getopt.add_option(     "arch",       arch,          GetOpt::REQUIRED_ARG).set_help("Architecture to build for.", "ARCH");
	getopt.add_option(     "conf-only",  conf_only,     GetOpt::NO_ARG).set_help("Stop after configuring packages.");
	getopt.add_option(     "full-paths", full_paths,    GetOpt::NO_ARG).set_help("Output full paths in analysis.");
	getopt.add_option(     "max-depth",  max_depth,     GetOpt::REQUIRED_ARG).set_help("Maximum depth to show in analysis.", "NUM");
	getopt.add_option(     "prefix",     prfx,          GetOpt::REQUIRED_ARG).set_help("Directory to install things to.", "DIR");
	getopt.add_option(     "warnings",   cmdline_warn,  GetOpt::REQUIRED_ARG).set_help("Compiler warnings to use.", "LIST");
	usagemsg=getopt.generate_usage(argv[0])+" [<target> ...]";
	helpmsg=getopt.generate_help();
	getopt(argc, argv);

	if(!analyze_mode.empty())
	{
		analyzer=new Analyzer(*this);

		if(analyze_mode=="deps")
			analyzer->set_mode(Analyzer::DEPS);
		else if(analyze_mode=="alldeps")
			analyzer->set_mode(Analyzer::ALLDEPS);
		else if(analyze_mode=="rebuild")
			analyzer->set_mode(Analyzer::REBUILD);
		else if(analyze_mode=="rdeps")
			analyzer->set_mode(Analyzer::RDEPS);
		else
			throw UsageError("Invalid analyze mode");

		analyzer->set_max_depth(max_depth);
		analyzer->set_full_paths(full_paths);
	}
	else if(!clean && !create_makefile)
		build=true;

	const vector<string> &args=getopt.get_args();
	for(vector<string>::const_iterator i=args.begin(); i!=args.end(); ++i)
	{
		string::size_type equal=i->find('=');
		if(equal!=string::npos)
			cmdline_options.insert(StringMap::value_type(i->substr(0, equal), i->substr(equal+1)));
		else
			cmdline_targets.push_back(*i);
	}

	if(cmdline_targets.empty())
		cmdline_targets.push_back("default");

	if(!work_dir.empty())
		FS::chdir(work_dir);

	cwd=FS::getcwd();

	utsname un;
	string sysname="native";
	if(uname(&un)==0)
		sysname=tolower(un.sysname);

	native_arch=&archs.insert(ArchMap::value_type(sysname, Architecture(*this, sysname, true))).first->second;
	native_arch->set_tool("CC",  "gcc");
	native_arch->set_tool("CXX", "g++");
	native_arch->set_tool("LD",  "gcc");
	native_arch->set_tool("LXX", "g++");
	native_arch->set_tool("AR",  "ar");

	load_build_file((FS::get_sys_data_dir(argv[0], "builder")/"builderrc").str());
	load_build_file((FS::get_user_data_dir("builder")/"rc").str());

	if(arch.empty())
		current_arch=native_arch;
	else
		current_arch=&get_architecture(arch);

	if(prfx.empty())
	{
		if(current_arch->is_native())
			prefix=(FS::get_home_dir()/"local").str();
		else
			prefix=(FS::get_home_dir()/"local"/current_arch->get_name()).str();
	}
	else
		prefix=FS::getcwd()/prfx;

	warnings.push_back("all");
	warnings.push_back("extra");
	warnings.push_back("shadow");
	warnings.push_back("pointer-arith");
	warnings.push_back("error");
	for(StringList::iterator i=cmdline_warn.begin(); i!=cmdline_warn.end(); ++i)
	{
		vector<string> warns=split(*i, ',');
		warnings.insert(warnings.end(), warns.begin(), warns.end());
	}

	pkg_path.push_back(cwd/".");
	pkg_path.push_back(cwd/"..");
}

Builder::~Builder()
{
	for(PackageMap::iterator i=packages.begin(); i!=packages.end(); ++i)
		delete i->second;
	for(TargetMap::iterator i=targets.begin(); i!=targets.end(); ++i)
		delete i->second;
	delete analyzer;
}

int Builder::main()
{
	if(prefix.str()!="/usr")
	{
		FS::Path pcdir=prefix/"lib"/"pkgconfig";
		if(const char *pcp=getenv("PKG_CONFIG_PATH"))
		{
			vector<string> path=split(pcp, ':');
			bool found=false;
			for(vector<string>::const_iterator i=path.begin(); (!found && i!=path.end()); ++i)
				found=(*i==pcdir.str());
			if(!found)
			{
				path.push_back(pcdir.str());
				setenv("PKG_CONFIG_PATH", join(path.begin(), path.end(), ":").c_str(), true);
			}
		}
		else
			setenv("PKG_CONFIG_PATH", pcdir.str().c_str(), true);
	}

	if(load_build_file(cwd/build_file))
	{
		if(help)
		{
			usage(0, "builder", false);
			return 0;
		}
		else
		{
			IO::print(IO::cerr, "No build info here.\n");
			return 1;
		}
	}

	main_pkg->configure(cmdline_options, conf_all?2:1);

	if(help)
	{
		usage(0, "builder", false);
		IO::print("\n");
		package_help();
		return 0;
	}

	if(!conf_only && create_targets())
		return 1;

	PackageList all_reqs=main_pkg->collect_requires();

	if(conf_only)
		return 0;

	if(verbose>=2)
		IO::print("Building on %s, for %s\n", native_arch->get_name(), current_arch->get_name());
	if(verbose>=1)
		IO::print("%d active packages, %d targets\n", all_reqs.size(), targets.size());
	if(verbose>=2)
	{
		for(PackageList::const_iterator i=all_reqs.begin(); i!=all_reqs.end(); ++i)
		{
			IO::print(" %s", (*i)->get_name());
			if(dynamic_cast<SourcePackage *>(*i))
				IO::print("*");
			unsigned count=0;
			unsigned to_be_built=0;
			for(TargetMap::iterator j=targets.begin(); j!=targets.end(); ++j)
				if(j->second->get_package()==*i)
				{
					++count;
					if(j->second->get_rebuild())
						++to_be_built;
				}
			if(count)
			{
				IO::print(" (%d targets", count);
				if(to_be_built)
					IO::print(", %d to be built", to_be_built);
				IO::print(")");
			}
			IO::print("\n");
		}
	}

	if(analyzer)
		analyzer->analyze();

	if(!problems.empty())
	{
		IO::print(IO::cerr, "The following problems were detected:\n");
		for(ProblemList::iterator i=problems.begin(); i!=problems.end(); ++i)
			IO::print(IO::cerr, "  %s: %s\n", i->package, i->descr);
		if(!analyzer)
			IO::print(IO::cerr, "Please fix them and try again.\n");
		return 1;
	}

	if(clean)
		exit_code=do_clean();
	else if(build)
		exit_code=do_build();

	return exit_code;
}

string Builder::run_pkgconfig(const string &pkg, const string &what)
{
	list<string> argv;
	argv.push_back("pkg-config");
	if(what=="cflags" || what=="libs")
		argv.push_back("--"+what);
	else if(what=="flags")
	{
		argv.push_back("--cflags");
		argv.push_back("--libs");
	}
	else
		argv.push_back("--variable="+what);
	argv.push_back(pkg);

	if(verbose>=4)
		IO::print("Running %s\n", join(argv.begin(), argv.end()));

	int status;
	string res=run_command(argv, &status);
	if(status)
		throw Exception(format("pkg-config for package %s failed", pkg));

	return res;
}

Package *Builder::get_package(const string &name)
{
	PackageMap::iterator i=packages.find(format("%s/%s", name, current_arch->get_name()));
	if(i==packages.end())
		i=packages.find(name);
	if(i!=packages.end())
		return i->second;

	FS::Path path=get_package_location(name);
	if(!path.empty() && !load_build_file(path/"Build"))
	{
		i=packages.find(name);
		if(i!=packages.end())
			return i->second;
	}

	Package *pkg=0;
	try
	{
		// Package source not found - create a binary package
		pkg=BinaryPackage::from_pkgconfig(*this, name);
	}
	catch(...)
	{
		problem(name, "not found");
	}

	packages.insert(PackageMap::value_type(name, pkg));

	return pkg;
}

Target *Builder::get_target(const string &n) const
{
	// XXX Used for getting targets by path.  get_target(const FS::Path &)?
	TargetMap::const_iterator i=targets.find(n);
	if(i!=targets.end())
		return i->second;
	return 0;
}

Target *Builder::get_header(const string &include, const FS::Path &from, const list<string> &path)
{
	string hash(8, 0);
	if(include[0]=='\"')
		update_hash(hash, from.str());
	for(list<string>::const_iterator i=path.begin(); i!=path.end(); ++i)
		update_hash(hash, *i);

	string id=hash+include;
	TargetMap::iterator i=includes.find(id);
	if(i!=includes.end())
		return i->second;

	static string cxx_ver;
	if(cxx_ver.empty())
	{
		StringList argv;
		argv.push_back(current_arch->get_tool("CXX"));
		argv.push_back("--version");
		if(RegMatch m=Regex("[0-9]\\.[0-9.]+").match(run_command(argv)))
		{
			cxx_ver=m[0].str;
			while(!cxx_ver.empty() && !FS::is_dir(FS::Path("/usr/include/c++")/cxx_ver))
			{
				string::size_type dot=cxx_ver.rfind('.');
				if(dot==string::npos)
					break;
				cxx_ver.erase(dot);
			}
			if(verbose>=5)
				IO::print("C++ version is %s\n", cxx_ver);
		}
		else
			cxx_ver="-";
	}

	string fn=include.substr(1);
	if(verbose>=5)
		IO::print("Looking for include %s from %s with path %s\n", fn, from, join(path.begin(), path.end()));

	StringList syspath;
	if(current_arch->is_native())
		syspath.push_back("/usr/include");
	else
		syspath.push_back("/usr/"+current_arch->get_prefix()+"/include");
	if(cxx_ver!="-")
		syspath.push_back((FS::Path("/usr/include/c++/")/cxx_ver).str());

	Target *tgt=0;
	if(include[0]=='\"')
		tgt=get_header(FS::Path(from)/fn);
	for(list<string>::const_iterator j=path.begin(); (!tgt && j!=path.end()); ++j)
		tgt=get_header(cwd/ *j/fn);
	for(list<string>::const_iterator j=syspath.begin(); (!tgt && j!=syspath.end()); ++j)
		tgt=get_header(FS::Path(*j)/fn);

	includes.insert(TargetMap::value_type(id, tgt));

	return tgt;
}

Target *Builder::get_library(const string &lib, const list<string> &path, LibMode mode)
{
	string hash(8, 0);
	for(list<string>::const_iterator i=path.begin(); i!=path.end(); ++i)
		update_hash(hash, *i);

	string id=hash+string(1, mode)+lib;
	TargetMap::iterator i=libraries.find(id);
	if(i!=libraries.end())
		return i->second;

	StringList syspath;
	if(current_arch->is_native())
	{
		syspath.push_back("/lib");
		syspath.push_back("/usr/lib");
	}
	else
		syspath.push_back("/usr/"+current_arch->get_prefix()+"/lib");

	if(verbose>=5)
		IO::print("Looking for library %s with path %s\n", lib, join(path.begin(), path.end()));

	Target *tgt=0;
	for(StringList::const_iterator j=path.begin(); (!tgt && j!=path.end()); ++j)
		tgt=get_library(lib, cwd/ *j, mode);
	for(StringList::iterator j=syspath.begin(); (!tgt && j!=syspath.end()); ++j)
		tgt=get_library(lib, *j, mode);

	libraries.insert(TargetMap::value_type(id, tgt));

	return tgt;
}

const Architecture &Builder::get_architecture(const string &arch) const
{
	ArchMap::const_iterator i=archs.find(arch);
	if(i==archs.end())
		throw KeyError("Unknown architecture", arch);

	return i->second;
}

void Builder::apply_profile_template(Config &config, const string &pt) const
{
	vector<string> parts=split(pt, '-');

	for(vector<string>::iterator i=parts.begin(); i!=parts.end(); ++i)
	{
		ProfileTemplateMap::const_iterator j=profile_tmpl.find(*i);
		if(j==profile_tmpl.end())
			continue;

		config.update(j->second);
	}
}

void Builder::problem(const string &p, const string &d)
{
	problems.push_back(Problem(p, d));
}

void Builder::add_target(Target *t)
{
	targets.insert(TargetMap::value_type(t->get_name(), t));
	new_tgts.push_back(t);
}

void Builder::usage(const char *reason, const char *argv0, bool brief)
{
	if(reason)
		IO::print(IO::cerr, "%s\n", reason);

	if(brief)
		IO::print(IO::cerr, "Usage: %s\n", usagemsg);
	else
	{
		IO::print(IO::cerr, "Builder 1.0\n\n");
		IO::print(IO::cerr, "Usage: %s [options] [<target> ...]\n\n", argv0);
		IO::print(IO::cerr, "Options:\n");
		IO::print(IO::cerr, helpmsg);
	}
}

FS::Path Builder::get_package_location(const string &name)
{
	if(verbose>=3)
		IO::print("Looking for package %s\n", name);

	try
	{
		// Try to get source directory with pkgconfig
		string srcdir=strip(run_pkgconfig(name, "source"));
		if(!srcdir.empty())
			return srcdir;
	}
	catch(...)
	{ }

	if(pkg_dirs.empty())
	{
		for(list<FS::Path>::const_iterator i=pkg_path.begin(); i!=pkg_path.end(); ++i)
		{
			list<string> files=list_files(*i);
			for(list<string>::const_iterator j=files.begin(); j!=files.end(); ++j)
			{
				FS::Path full=*i / *j;
				if(FS::exists(full/"Build"))
					pkg_dirs.push_back(full);
			}
		}
		if(verbose>=3)
			IO::print("%d packages found in path\n", pkg_dirs.size());
	}

	bool msp=!name.compare(0, 3, "msp");
	for(list<FS::Path>::const_iterator i=pkg_dirs.begin(); i!=pkg_dirs.end(); ++i)
	{
		string base=basename(*i);
		unsigned dash=base.rfind('-');

		if(!base.compare(0, dash, name))
			return *i;
		else if(msp && !base.compare(0, dash-3, name, 3, string::npos))
			return *i;
	}

	return FS::Path();
}

int Builder::load_build_file(const FS::Path &fn)
{
	try
	{
		IO::BufferedFile in(fn.str());

		if(verbose>=3)
			IO::print("Reading %s\n", fn);

		DataFile::Parser parser(in, fn.str());
		Loader loader(*this, fn.subpath(0, fn.size()-1));
		loader.load(parser);
	}
	catch(const IO::FileNotFound &)
	{
		return -1;
	}

	return 0;
}

int Builder::create_targets()
{
	Target *world=new VirtualTarget(*this, "world");

	Target *def_tgt=new VirtualTarget(*this, "default");
	world->add_depend(def_tgt);

	Target *install=new VirtualTarget(*this, "install");
	world->add_depend(install);

	Target *tarballs=new VirtualTarget(*this, "tarballs");
	world->add_depend(tarballs);

	PackageList all_reqs=main_pkg->collect_requires();
	for(PackageList::iterator i=all_reqs.begin(); i!=all_reqs.end(); ++i)
	{
		SourcePackage *spkg=dynamic_cast<SourcePackage *>(*i);
		if(!spkg)
			continue;

		const ComponentList &components=spkg->get_components();
		for(ComponentList::const_iterator j=components.begin(); j!=components.end(); ++j)
			j->create_targets();

		if(spkg->get_install_flags()&(SourcePackage::LIB|SourcePackage::INCLUDE))
		{
			PkgConfig *pc=new PkgConfig(*this, *spkg);
			install->add_depend(new Install(*this, *spkg, *pc));
		}
	}

	// Find dependencies until no new targets are created
	while(!new_tgts.empty())
	{
		Target *tgt=new_tgts.front();
		new_tgts.erase(new_tgts.begin());
		tgt->find_depends();
		if(!tgt->get_depends_ready())
			new_tgts.push_back(tgt);
	}

	// Apply what-ifs
	for(StringList::iterator i=what_if.begin(); i!=what_if.end(); ++i)
	{
		Target *tgt=get_target((cwd/ *i).str());
		if(!tgt)
		{
			IO::print(IO::cerr, "Unknown what-if target %s\n", *i);
			return -1;
		}
		tgt->touch();
	}

	// Make the cmdline target depend on all targets mentioned on the command line
	Target *cmdline=new VirtualTarget(*this, "cmdline");
	bool build_world=false;
	for(list<string>::iterator i=cmdline_targets.begin(); i!=cmdline_targets.end(); ++i)
	{
		Target *tgt=get_target(*i);
		if(!tgt)
			tgt=get_target((cwd/ *i).str());
		if(!tgt)
		{
			IO::print("I don't know anything about %s\n", *i);
			return -1;
		}
		if(tgt==world)
			build_world=true;
		cmdline->add_depend(tgt);
	}

	cmdline->prepare();

	for(PackageMap::iterator i=packages.begin(); i!=packages.end(); ++i)
		if(SourcePackage *spkg=dynamic_cast<SourcePackage *>(i->second))
			spkg->get_deps_cache().save();

	return 0;
}

Target *Builder::get_header(const FS::Path &fn)
{
	Target *tgt=get_target(fn.str());
	if(tgt) return tgt;

	if(FS::is_reg(fn))
	{
		tgt=new SystemHeader(*this, fn.str());
		return tgt;
	}
	return 0;
}

Target *Builder::get_library(const string &lib, const FS::Path &path, LibMode mode)
{
	// Populate a list of candidate filenames
	StringList candidates;

	if(mode!=ALL_STATIC)
	{
		if(current_arch->get_name()=="win32")
		{
			candidates.push_back("lib"+lib+".dll");
			candidates.push_back(lib+".dll");
		}
		else
			candidates.push_back("lib"+lib+".so");
	}

	/* Static libraries are always considered, since sometimes shared versions
	may not be available */
	candidates.push_back("lib"+lib+".a");
	if(current_arch->get_name()=="win32")
		candidates.push_back("lib"+lib+".dll.a");

	for(StringList::iterator i=candidates.begin(); i!=candidates.end(); ++i)
	{
		string full=(path/ *i).str();
		Target *tgt=get_target(full);

		if(tgt)
		{
			Target *real_tgt=tgt;
			if(Install *inst=dynamic_cast<Install *>(tgt))
				real_tgt=&inst->get_source();

			/* Ignore dynamic libraries from local packages unless library mode is
			DYNAMIC */
			if(dynamic_cast<SharedLibrary *>(real_tgt) && mode!=DYNAMIC)
				continue;
			else if(tgt)
				return tgt;
		}
		else if(FS::is_reg(full))
		{
			tgt=new SystemLibrary(*this, full);
			return tgt;
		}
	}

	return 0;
}

int Builder::do_build()
{
	Target *cmdline=get_target("cmdline");

	unsigned total=0;
	for(map<string, Target *>::const_iterator i=targets.begin(); i!=targets.end(); ++i)
		if(i->second->is_buildable() && i->second->get_rebuild())
			++total;

	if(!total)
	{
		IO::print("Already up to date\n");
		return 0;
	}
	if(verbose>=1)
		IO::print("Will build %d target%s\n", total, (total!=1 ? "s" : ""));

	vector<Action *> actions;

	unsigned count=0;

	bool fail=false;
	bool finish=false;

	while(!finish)
	{
		if(actions.size()<jobs && !fail)
		{
			Target *tgt=cmdline->get_buildable_target();
			if(tgt)
			{
				Action *action=tgt->build();
				if(action)
					actions.push_back(action);

				if(show_progress)
					IO::print("%d of %d target%s built\033[1G", count, total, (total!=1 ? "s" : ""));
			}
			else if(actions.empty())
				finish=true;
		}
		else
			Time::sleep(10*Time::msec);

		for(unsigned i=0; i<actions.size();)
		{
			int status=actions[i]->check();
			if(status>=0)
			{
				++count;

				delete actions[i];
				actions.erase(actions.begin()+i);
				if(status>0)
					fail=true;
				if(actions.empty() && fail)
					finish=true;
			}
			else
				++i;
		}
	}

	if(show_progress)
		IO::print("\033[K");
	if(fail)
		IO::print("Build failed\n");
	else if(show_progress)
		IO::print("Build complete\n");

	return fail;
}

int Builder::do_clean()
{
	// Cleaning doesn't care about ordering, so a simpler method can be used

	set<Target *> clean_tgts;
	TargetList queue;
	queue.push_back(get_target("cmdline"));

	while(!queue.empty())
	{
		Target *tgt=queue.front();
		queue.erase(queue.begin());

		if(tgt->is_buildable() && (tgt->get_package()==main_pkg || clean>=2))
			clean_tgts.insert(tgt);

		const TargetList &deps=tgt->get_depends();
		for(TargetList::const_iterator i=deps.begin(); i!=deps.end(); ++i)
			if(!clean_tgts.count(*i))
				queue.push_back(*i);
	}

	for(set<Target *>::iterator i=clean_tgts.begin(); i!=clean_tgts.end(); ++i)
		if(FileTarget *ft=dynamic_cast<FileTarget *>(*i))
		{
			Action *action=new Unlink(*this, *ft);
			while(action->check()<0) ;
			delete action;
		}

	return 0;
}

void Builder::package_help()
{
	const Config &config=main_pkg->get_config();
	const Config::OptionMap &options=config.get_options();

	IO::print("Required packages:\n  ");
	const PackageList &requires=main_pkg->get_requires();
	for(PackageList::const_iterator i=requires.begin(); i!=requires.end(); ++i)
	{
		if(i!=requires.begin())
			IO::print(", ");
		IO::print((*i)->get_name());
	}
	IO::print("\n\nPackage configuration:\n");
	for(Config::OptionMap::const_iterator i=options.begin(); i!=options.end(); ++i)
	{
		const Config::Option &opt=i->second;
		IO::print("  %s: %s (%s)", opt.name, opt.descr, opt.value);
		if(opt.value!=opt.defv)
			IO::print(" [%s]", opt.defv);
		IO::print("\n");
	}
}

Application::RegApp<Builder> Builder::reg;
string Builder::usagemsg;
string Builder::helpmsg;


Builder::Loader::Loader(Builder &b, const FS::Path &s):
	bld(b),
	src(s)
{
	add("architecture", &Loader::architecture);
	add("binary_package", &Loader::binpkg);
	add("profile", &Loader::profile);
	add("package", &Loader::package);
}

void Builder::Loader::architecture(const string &n)
{
	Architecture arch(bld, n);
	load_sub(arch);
	bld.archs.insert(ArchMap::value_type(n, arch));
}

void Builder::Loader::binpkg(const string &n)
{
	BinaryPackage *pkg=new BinaryPackage(bld, n);
	load_sub(*pkg);
	bld.packages.insert(PackageMap::value_type(n, pkg));
}

void Builder::Loader::profile(const string &n)
{
	StringMap prf;
	ProfileLoader ldr(prf);
	load_sub_with(ldr);
	bld.profile_tmpl.insert(ProfileTemplateMap::value_type(n, prf));
}

void Builder::Loader::package(const string &n)
{
	SourcePackage *pkg=new SourcePackage(bld, n, src);
	if(!bld.main_pkg)
		bld.main_pkg=pkg;

	load_sub(*pkg);
	bld.packages.insert(PackageMap::value_type(n, pkg));
}


Builder::ProfileLoader::ProfileLoader(StringMap &p):
	profile(p)
{
	add("option", &ProfileLoader::option);
}

void Builder::ProfileLoader::option(const string &o, const string &v)
{
	profile.insert(StringMap::value_type(o, v));
}
