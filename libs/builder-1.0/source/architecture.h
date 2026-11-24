/* $Id: architecture.h 84 2009-02-19 16:43:37Z tdb $

This file is part of builder
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef ARCHITECTURE_H_
#define ARCHITECTURE_H_

#include <msp/datafile/loader.h>
#include "misc.h"

class Builder;

// XXX Add lib/exe prefix/suffix fields.  Some archs may need multiple alternatives, how to handle this?
class Architecture
{
public:
	class Loader: public Msp::DataFile::Loader
	{
	private:
		Architecture &arch;

	public:
		Loader(Architecture &);
		Architecture &get_object() { return arch; }
	private:
		void tool(const std::string &t, const std::string &p);
	};

private:
	Builder &builder;
	std::string name;
	bool native;
	std::string prefix;
	StringMap tools;

public:
	Architecture(Builder &b, const std::string &n, bool a=false);
	void set_tool(const std::string &t, const std::string &p);
	std::string get_tool(const std::string &t) const;
	const std::string &get_name() const { return name; }
	bool is_native() const { return native; }
	const std::string &get_prefix() const { return prefix; }
};

typedef std::map<std::string, Architecture> ArchMap;

#endif
