/* $Id: analyzer.cpp 97 2009-09-20 16:19:05Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/fs/utils.h>
#include <msp/io/print.h>
#include "analyzer.h"
#include "builder.h"
#include "install.h"
#include "objectfile.h"
#include "package.h"
#include "sourcefile.h"
#include "target.h"

using namespace std;
using namespace Msp;

Analyzer::Analyzer(Builder &b):
	builder(b),
	mode(DEPS),
	max_depth(0),
	full_paths(false)
{ }

void Analyzer::analyze()
{
	table.clear();

	TableRow row;
	row.push_back("Name");
	row.push_back("Package");
	row.push_back("Type");
	row.push_back("Rebuild");
	table.push_back(row);
	
	build_depend_table(*builder.get_target("cmdline"), 0);

	print_table();
}

void Analyzer::build_depend_table(Target &tgt, unsigned depth)
{
	if(mode!=REBUILD && mode!=ALLDEPS)
	{
		// Skip trivial targets
		if(ObjectFile *obj=dynamic_cast<ObjectFile *>(&tgt))
			return build_depend_table(obj->get_source(), depth);
		else if(Install *inst=dynamic_cast<Install *>(&tgt))
			return build_depend_table(inst->get_source(), depth);
	}
	else if(mode==REBUILD && !tgt.get_rebuild())
		/* All targets that depend on to-be-built targets will be rebuilt
		themselves, so we cn stop here. */
		return;
	
	TableRow row;

	string fn;
	if(full_paths)
		fn=tgt.get_name();
	else
		fn=FS::basename(tgt.get_name());
	row.push_back(string(depth*2, ' ')+fn);

	const Package *pkg=tgt.get_package();
	if(pkg)
		row.push_back(pkg->get_name());
	else
		row.push_back("");
	
	row.push_back(tgt.get_type());
	
	if(tgt.get_rebuild())
	{
		if(tgt.get_rebuild_reason().empty())
			row.push_back("Yes (no reason)");
		else
			row.push_back(tgt.get_rebuild_reason());
	}

	table.push_back(row);

	if(!max_depth || depth<max_depth-1)
	{
		const TargetList &depends=tgt.get_depends();
		//XXX If we want to sort the targets, we need to take the value of full_paths into account
		//depends.sort(target_order);
		for(TargetList::const_iterator i=depends.begin(); i!=depends.end(); ++i)
			build_depend_table(**i, depth+1);
	}
}

void Analyzer::print_table() const
{
	vector<string::size_type> col_width;

	// Determine column widths
	for(Table::const_iterator i=table.begin(); i!=table.end(); ++i)
	{
		if(col_width.size()<i->size())
			col_width.resize(i->size(), 0);
		for(unsigned j=0; j<i->size(); ++j)
			col_width[j]=max(col_width[j], (*i)[j].size());
	}

	for(Table::const_iterator i=table.begin(); i!=table.end(); ++i)
	{
		string line;
		for(unsigned j=0; j<i->size(); ++j)
		{
			if(j>0)
				line+="  ";
			line+=lexical_cast((*i)[j], Fmt("%-s").width(col_width[j]));
		}
		IO::print("%s\n", line);
	}
}

bool Analyzer::target_order(Target *t1, Target *t2)
{ return t1->get_name()<t2->get_name(); }
