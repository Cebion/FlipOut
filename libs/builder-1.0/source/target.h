/* $Id: target.h 93 2009-08-26 11:16:55Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef TARGET_H_
#define TARGET_H_

#include <list>
#include <map>
#include <set>
#include <string>
#include <msp/time/timestamp.h>

class Action;
class Builder;
class Package;

class Target;
typedef std::list<Target *> TargetList;

/**
Targets make up the build graph.  This class is a base for all target types and
handles many common tasks.  Most targets are associated with a file.
*/
class Target
{
protected:
	Builder &builder;
	const Package *package;
	std::string name;
	Msp::Time::TimeStamp mtime;

	bool buildable;
	bool building;
	bool rebuild;
	std::string rebuild_reason;

	TargetList depends;
	TargetList rdepends;
	bool deps_ready;

	bool preparing;
	bool prepared;

	Target(Builder &, const Package *, const std::string &);
public:
	virtual ~Target() { }

	virtual const char *get_type() const=0;
	const std::string &get_name() const { return name; }
	const Package *get_package() const { return package; }
	const Msp::Time::TimeStamp &get_mtime() const { return mtime; }

	/**
	Tries to locate a target that will help getting this target built.  If all
	dependencies are up-to-date, returns this target.  If there are no targets
	ready to be built (maybe because they are being built right now), returns 0.
	*/
	Target *get_buildable_target();

	bool is_buildable() const { return buildable; }
	bool get_rebuild() const { return rebuild; }
	const std::string &get_rebuild_reason() const { return rebuild_reason; }
	void add_depend(Target *);
	const TargetList &get_depends() const { return depends; }
	bool get_depends_ready() const { return deps_ready; }

	/**
	Finds dependencies for the target.  When all dependencies have been found,
	the function should set deps_ready to true.
	*/
	virtual void find_depends() { deps_ready=true; }

	/**
	Prepares the target by recursively preparing dependencies, then checking
	whether rebuilding is needed.  A flag is used to prevent unnecessary
	executions.
	*/
	virtual void prepare();

	/**
	Starts building the target.  Returns the Action used for building.
	*/
	Action *build();

	/**
	Changes the mtime of the target to the current time.
	*/
	void touch();
protected:
	void mark_rebuild(const std::string &);

	/**
	Checks if the target needs to be rebuilt and why.
	*/
	virtual void check_rebuild();

	/**
	Creates and returns an Action suitable for building this target.
	*/
	virtual Action *create_action() =0;

	/**
	Handler for the build_done signal of Action.
	*/
	virtual void build_done();
};

typedef std::map<std::string, Target *> TargetMap;

#endif
