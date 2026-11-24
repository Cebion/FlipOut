/* $Id: condition.h 84 2009-02-19 16:43:37Z tdb $

This file is part of builder
Copyright © 2007, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef CONDITION_H_
#define CONDITION_H_

#include <msp/datafile/loader.h>
#include "buildinfo.h"
#include "misc.h"

class Config;
class SourcePackage;

class Condition
{
public:
	class Loader: public Msp::DataFile::Loader
	{
	private:
		Condition &cond;

	public:
		Loader(Condition &);
	private:
		void require(const std::string &);
		void build_info();
	};

private:
	SourcePackage &pkg;
	StringMap expression;
	StringList requires;
	BuildInfo build_info;

public:
	Condition(SourcePackage &, const std::string &);
	const StringList &get_requires() const { return requires; }
	const BuildInfo &get_build_info() const { return build_info; }
	bool eval();
};
typedef std::list<Condition> ConditionList;

#endif
