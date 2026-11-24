/* $Id: builder.h 91 2009-05-11 20:43:21Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef BUILDER_H_
#define BUILDER_H_

#include <list>
#include <map>
#include <string>
#include <msp/core/application.h>
#include <msp/datafile/loader.h>
#include <msp/fs/path.h>
#include "architecture.h"
#include "config.h"
#include "misc.h"
#include "problem.h"
#include "target.h"

class Analyzer;
class Config;
class Package;
class SourcePackage;

/**
The main application class.  Controls and owns everything.  Rules the world.
*/
class Builder: public Msp::Application
{
private:
	class Loader: public Msp::DataFile::Loader
	{
	private:
		Builder &bld;
		Msp::FS::Path src;

	public:
		Loader(Builder &, const Msp::FS::Path &);
	private:
		void architecture(const std::string &);
		void binpkg(const std::string &);
		void profile(const std::string &);
		void package(const std::string &);
	};

	class ProfileLoader: public Msp::DataFile::Loader
	{
	private:
		StringMap &profile;

	public:
		ProfileLoader(StringMap &);
	private:
		void option(const std::string &, const std::string &);
	};

	typedef std::list<Package *> PackageList;
	typedef std::map<std::string, Package *> PackageMap;
	typedef std::map<std::string, StringMap> ProfileTemplateMap;

	StringList cmdline_targets;
	StringMap cmdline_options;
	Msp::FS::Path cwd;

	PackageMap packages;
	SourcePackage *main_pkg;
	PathList pkg_path;
	PathList pkg_dirs;

	TargetMap targets;
	TargetList new_tgts;
	TargetMap includes;
	TargetMap libraries;

	ArchMap archs;
	Architecture *native_arch;
	const Architecture *current_arch;
	ProfileTemplateMap profile_tmpl;

	ProblemList problems;
	Analyzer *analyzer;
	bool build;
	unsigned clean;
	bool dry_run;
	bool help;
	unsigned verbose;
	bool show_progress;
	std::string build_file;
	unsigned jobs;
	StringList what_if;
	bool conf_all;
	bool conf_only;
	bool build_all;
	bool create_makefile;
	Msp::FS::Path prefix;
	StringList warnings;

	static Msp::Application::RegApp<Builder> reg;
	static std::string usagemsg;
	static std::string helpmsg;

public:
	Builder(int, char **);
	~Builder();

	int main();
	unsigned get_verbose() const { return verbose; }
	bool get_dry_run() const { return dry_run; }
	bool get_build_all() const { return build_all; }

	/** Gets a package by name, possibly creating it.  Returns 0 if the package
	could not be located. */
	Package *get_package(const std::string &);

	SourcePackage *get_main_package() const { return main_pkg; }

	std::string run_pkgconfig(const std::string &, const std::string &);

	/** Looks up a target by name.  Returns 0 if no such target exists. */
	Target *get_target(const std::string &) const;

	const TargetMap &get_targets() const { return targets; }

	/** Tries to locate a header based on location of including file and include
	path.  Considers known targets as well as existing files.  If a matching
	target is not found but a file exists, a new SystemHeader target will be
	created and returned. */
	Target *get_header(const std::string &, const Msp::FS::Path &, const StringList &);

	/** Tries to locate a library in a library path.  The library name should be
	the same as would be given to the linker with -l, i.e. without the "lib"
	prefix or extension.  Considers known targets as well as existing files.  If
	a matching target is not found but a file exists, a new SystemLibrary target
	will be created and returned. */
	Target *get_library(const std::string &, const StringList &, LibMode);

	const Msp::FS::Path &get_cwd() const { return cwd; }
	const Architecture &get_architecture(const std::string &) const;
	const Architecture &get_current_arch() const { return *current_arch; }
	const Architecture &get_native_arch() const { return *native_arch; }
	const Msp::FS::Path &get_prefix() const { return prefix; }
	const StringList &get_warnings() const { return warnings; }
	void apply_profile_template(Config &, const std::string &) const;

	/** Adds a target to both the target map and the new target queue.  Called
	from Target constructor. */
	void add_target(Target *);

	void problem(const std::string &, const std::string &);

	static void usage(const char *, const char *, bool);

private:
	/** Determines the source directory of a package.  Pkg-config is consulted
	first, and if it fails, the package path is searched for matches. */
	Msp::FS::Path get_package_location(const std::string &);

	/** Loads a build file.  Returns 0 on success or -1 if the file could not be
	opened. */
	int load_build_file(const Msp::FS::Path &);

	/** Creates targets for all packages and prepares them for building.
	Returns 0 if everything went ok, -1 if something bad happened and a build
	shouldn't be attempted. */
	int create_targets();

	/**
	Check if a header exists, either as a target or a file.  Returns an existing
	target of one was found, or a new SystemHeader target if there was no known
	target but the file exists.
	*/
	Target *get_header(const Msp::FS::Path &);

	Target *get_library(const std::string &, const Msp::FS::Path &, LibMode);

	/** Supervises the build process, starting new actions when slots become
	available. */
	int do_build();

	/** Cleans buildable targets.  If clean is 1, cleans only the default
	package.  If clean is 2 or greater, cleans all buildable packages.
	*/
	int do_clean();

	int do_create_makefile();

	/** Prints out information about the default package. */
	void package_help();
};

#endif
