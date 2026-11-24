/* $Id$

This file is part of builder
Copyright © 2007, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef DEPENDENCYCACHE_H_
#define DEPENDENCYCACHE_H_

#include <msp/time/timestamp.h>
#include "misc.h"

class Package;

/**
Stores dependencies to avoid expensive operations during DAG building phase.
The dependencies are stored in a map with target name as key and a list of
strings as value.  The targets are free to store whatever they want here.
*/
class DependencyCache
{
private:
	typedef std::map<std::string, StringList> DepsMap;

	SourcePackage &package;
	DepsMap deps;
	Msp::Time::TimeStamp mtime;
	bool changed;

public:
	DependencyCache(SourcePackage &p);
	void set_deps(const std::string &tgt, const StringList &d);
	const StringList &get_deps(const std::string &tgt) const;
	const Msp::Time::TimeStamp &get_mtime() const { return mtime; }
	void load();

	/**
	Saves the depencency cache.  Does nothing if the cache is empty or nothing
	has changed. */
	void save() const;
};

#endif
