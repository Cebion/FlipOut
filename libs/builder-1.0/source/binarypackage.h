/* $Id: binarypackage.h 84 2009-02-19 16:43:37Z tdb $

This file is part of builder
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef BINARYPACKAGE_H_
#define BINARYPACKAGE_H_

#include "package.h"

/**
Represents a package that is installed on the system, but can't be built by
Builder.
*/
class BinaryPackage: public Package
{
public:
	class Loader: public Package::Loader
	{
	public:
		Loader(BinaryPackage &);
		BinaryPackage &get_object() { return static_cast<BinaryPackage &>(pkg); }
	private:
		void build_info();
	};

private:
	bool need_path;
	Msp::FS::Path path;

public:
	BinaryPackage(Builder &, const std::string &);

	/**
	Sets the path where the package's files were installed.  This is only useful
	if the package doesn't use pkg-config.
	*/
	void set_path(const Msp::FS::Path &);

	bool get_need_path() const { return need_path; }

	static BinaryPackage *from_pkgconfig(Builder &, const std::string &);
private:
	virtual void create_build_info();
};

#endif
