/* $Id: analyzer.h 84 2009-02-19 16:43:37Z tdb $

This file is part of builder
Copyright © 2006-2007, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef ANALYZER_H_
#define ANALYZER_H_

#include <list>
#include <string>
#include <vector>

class Builder;
class Target;

/**
Performs various kinds of dependency analysis on the build tree.
*/
class Analyzer
{
public:
	enum Mode
	{
		DEPS,     //< Skip over "trivial" targets such as Install and Compile
		ALLDEPS,  //< Print out absolutely every target
		REBUILD,  //< Print targets that are going to be rebuilt
		RDEPS     //< Print targets that depend on the given targets (NYI)
	};

private:
	typedef std::vector<std::string> TableRow;
	typedef std::list<TableRow> Table;

	Builder &builder;
	Mode mode;
	Table table;
	unsigned max_depth;
	bool full_paths;

public:
	Analyzer(Builder &);
	void set_mode(Mode m) { mode=m; }
	void set_max_depth(unsigned m) { max_depth=m; }
	void set_full_paths(bool f) { full_paths=f; }

	/**
	Performs the analysis and prints out the resulting dependency tree.
	*/
	void analyze();

private:
	/**
	Adds rows to the table for the given target and its dependencies.
	*/
	void build_depend_table(Target &, unsigned);

	/**
	Prints out the table that resulted from the analysis.
	*/
	void print_table() const;

	static bool target_order(Target *, Target *);
};

#endif
