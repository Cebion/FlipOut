/* $Id: component.h 98 2009-09-20 17:34:22Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <string>
#include <msp/datafile/loader.h>
#include <msp/fs/path.h>
#include "buildinfo.h"
#include "misc.h"
#include "package.h"

class SourcePackage;

/**
Components specify things to be built.  Each component may build one binary (it
may also build none), as well as install a bunch of headers.  Components inherit
dependencies and build info from the package they belong to, and may also add
their own.
*/
class Component
{
public:
	class Loader: public Msp::DataFile::Loader
	{
	private:
		Component &comp;
		std::string inst_hdr;

	public:
		Loader(Component &);
		Component &get_object() { return comp; }
	private:
		virtual void finish();
		void source(const std::string &);
		void require(const std::string &);
		void modular();
		void host(const std::string &);
		void install_headers(const std::string &);
		void build_info();
	};

	enum Type
	{
		HEADERS,
		LIBRARY,
		PROGRAM,
		MODULE,
		DATAFILE,
		INSTALL,
		TARBALL
	};

protected:
	SourcePackage &pkg;
	Type type;
	std::string name;
	PathList sources;
	bool install;
	const Component *module_host;
	bool modular;
	BuildInfo build_info;
	PackageList requires;
	bool deflt;

public:
	Component(SourcePackage &, Type, const std::string &);
	const SourcePackage &get_package() const { return pkg; }
	Type get_type() const { return type; }
	const std::string &get_name() const { return name; }
	const PathList &get_sources() const { return sources; }
	const BuildInfo &get_build_info() const { return build_info; }
	bool get_install() const { return install; }
	bool is_modular() const { return modular; }
	const PackageList &get_requires() const { return requires; }
	bool is_default() const { return deflt; }

	/** Prepares the build information for building.  Pulls build info from the
	parent and dependency packages, and adds any component-specific flags. */
	void create_build_info();

	void create_targets() const;

protected:
	/** Returns a list of all source files for the component. */
	PathList collect_source_files() const;
};

typedef std::list<Component> ComponentList;

#endif
