/* $Id: architecture.cpp 84 2009-02-19 16:43:37Z tdb $

This file is part of builder
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "architecture.h"
#include "builder.h"

using namespace std;
using namespace Msp;

Architecture::Architecture(Builder &b, const string &n, bool a):
	builder(b),
	name(n),
	native(a)
{ }

void Architecture::set_tool(const string &t, const string &p)
{
	tools[t]=p;
}

std::string Architecture::get_tool(const string &t) const
{
	StringMap::const_iterator i=tools.find(t);
	if(i!=tools.end())
	{
		if(i->second[0]=='-')
			return prefix+i->second;
		else
			return i->second;
	}

	if(!native)
	{
		const Architecture &native_arch=builder.get_native_arch();
		return prefix+"-"+native_arch.get_tool(t);
	}
	else
		throw KeyError("Unknown tool");
}


Architecture::Loader::Loader(Architecture &a):
	arch(a)
{
	add("prefix", &Architecture::prefix);
	add("tool",   &Loader::tool);
}

void Architecture::Loader::tool(const string &t, const string &p)
{
	arch.tools[t]=p;
}
