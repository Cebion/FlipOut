/* $Id: package.h 84 2009-02-19 16:43:37Z tdb $

This file is part of builder
Copyright © 2006-2007, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef PACKAGE_H_
#define PACKAGE_H_

#include <list>
#include <string>
#include <msp/datafile/loader.h>
#include "buildinfo.h"

class Builder;
class Package;

typedef std::list<Package *> PackageList;

/**
A package is a distributable piece of software.  Package information may be
obtained in several ways: Build files of source packages, pkg-config for binary
packages and the builderrc file for binary packages with no pkg-config support.
*/
class Package
{
public:
	class Loader: public Msp::DataFile::Loader
	{
	public:
		Loader(Package &);
		Package &get_object() { return pkg; }
	protected:
		Package &pkg;

		void require(const std::string &);
	};

protected:
	Builder &builder;

	std::string name;

	PackageList requires;
	BuildInfo export_binfo;
	bool conf_done;

	bool use_pkgconfig;

	Package(Builder &, const std::string &);
public:
	virtual ~Package() { }

	const std::string &get_name() const { return name; }
	Builder &get_builder() const { return builder; }
	const PackageList &get_requires() const { return requires; }

	/** Returns a list of all directly and indirectly required packages,
	including the package itself. */
	PackageList collect_requires();

	const BuildInfo &get_exported_binfo() const { return export_binfo; }

	/// Indicates whether or not this package supports pkg-config
	bool get_use_pkgconfig() const { return use_pkgconfig; }

	/** Processes configuration options that were most likely obtained from the
	command line. */
	void configure(const StringMap &, unsigned);
protected:
	virtual void do_configure(const StringMap &, unsigned) { }
	virtual void create_build_info() { }
};

#endif
