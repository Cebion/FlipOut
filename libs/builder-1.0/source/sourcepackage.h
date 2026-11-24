/* $Id: sourcepackage.h 96 2009-09-11 22:48:06Z tdb $

This file is part of builder
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef SOURCEPACKAGE_H_
#define SOURCEPACKAGE_H_

#include <string>
#include "buildinfo.h"
#include "component.h"
#include "condition.h"
#include "config.h"
#include "dependencycache.h"
#include "feature.h"
#include "package.h"

class Builder;

/**
A package that can be built by Builder.
*/
class SourcePackage: public Package
{
public:
	enum InstallFlags
	{
		INCLUDE=1,
		BIN=2,
		LIB=4,
		DATA=8
	};

	class Loader: public Package::Loader
	{
	public:
		Loader(Package &);
		SourcePackage &get_object() { return static_cast<SourcePackage &>(pkg); }
	private:
		virtual void finish();
		void feature(const std::string &, const std::string &);
		template<Component::Type>
		void component(const std::string &);
		void condition(const std::string &);
		void build_info();
		void tarball(const std::string &);
		void tar_file(const std::string &);
	};

private:
	std::string version;
	std::string description;

	Msp::FS::Path source;
	PackageList base_reqs;
	FeatureList features;
	BuildInfo build_info;
	ConditionList conditions;
	ComponentList components;
	Config config;
	bool conf_done;
	mutable DependencyCache deps_cache;

public:
	SourcePackage(Builder &, const std::string &, const Msp::FS::Path &);
	const std::string &get_name() const { return name; }
	const std::string &get_version() const { return version; }
	const std::string &get_description() const { return description; }
	const Msp::FS::Path &get_source() const { return source; }
	Msp::FS::Path get_temp_dir() const;
	Msp::FS::Path get_out_dir() const;
	const ComponentList &get_components() const { return components; }
	const Config &get_config() const { return config; }
	const BuildInfo &get_build_info() const { return build_info; }
	const BuildInfo &get_exported_binfo() const { return export_binfo; }
	Builder &get_builder() const { return builder; }

	/** Returns a bitmask indicating which kinds of things the components of
	this package install. */
	unsigned get_install_flags();

	LibMode get_library_mode() const;
	DependencyCache &get_deps_cache() const { return deps_cache; }
private:
	virtual void do_configure(const StringMap &, unsigned);

	/** Initializes configuration options. */
	void init_config();

	/** Fills in build info based on configuration.  All required packages must be
	configured when this is called. */
	virtual void create_build_info();
};

#endif
