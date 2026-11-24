/* $Id: target.cpp 93 2009-08-26 11:16:55Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/fs/stat.h>
#include <msp/fs/utils.h>
#include <msp/time/utils.h>
#include "action.h"
#include "builder.h"
#include "filetarget.h"
#include "package.h"
#include "sourcepackage.h"
#include "target.h"

using namespace std;
using namespace Msp;

Target::Target(Builder &b, const Package *p, const string &n):
	builder(b),
	package(p),
	name(n),
	buildable(false),
	building(false),
	rebuild(false),
	deps_ready(false),
	preparing(false),
	prepared(false)
{
	builder.add_target(this);
}

Target *Target::get_buildable_target()
{
	if(!rebuild)
		return 0;

	bool self_ok=!building;
	for(TargetList::iterator i=depends.begin(); i!=depends.end(); ++i)
	{
		Target *tgt=(*i)->get_buildable_target();
		if(tgt)
			return tgt;
		else if((*i)->get_rebuild())
			self_ok=false;
	}

	if(self_ok)
		return this;

	return 0;
}

void Target::add_depend(Target *dep)
{
	if(dep==this)
		throw InvalidParameterValue("A target can't depend on itself");
	depends.push_back(dep);
	dep->rdepends.push_back(this);
}

void Target::prepare()
{
	if(prepared)
		return;
	if(preparing)
	{
		builder.problem((package ? package->get_name() : string()), "Dependency cycle detected at "+name);
		return;
	}

	preparing=true;
	for(TargetList::iterator i=depends.begin(); i!=depends.end(); ++i)
		(*i)->prepare();

	check_rebuild();
	preparing=false;
	prepared=true;
}

Action *Target::build()
{
	if(!buildable)
	{
		rebuild=false;
		return 0;
	}

	if(FileTarget *ft=dynamic_cast<FileTarget *>(this))
		if(!builder.get_dry_run() && FS::exists(ft->get_path()))
			FS::unlink(ft->get_path());

	Action *action=create_action();
	if(action)
	{
		action->signal_done.connect(sigc::mem_fun(this, &Target::build_done));

		building=true;
	}

	return action;
}

void Target::touch()
{
	mtime=Time::now();
}

void Target::mark_rebuild(const std::string &reason)
{
	rebuild=true;
	rebuild_reason=reason;
}

void Target::check_rebuild()
{
	if(!buildable)
		return;

	if(builder.get_build_all())
		mark_rebuild("Rebuilding everything");
	else if(!mtime)
		mark_rebuild("Does not exist");
	else
	{
		for(TargetList::iterator i=depends.begin(); (i!=depends.end() && !rebuild); ++i)
		{
			if((*i)->get_mtime()>mtime)
				mark_rebuild(FS::basename((*i)->get_name())+" has changed");
			else if((*i)->get_rebuild())
				mark_rebuild(FS::basename((*i)->get_name())+" needs rebuilding");
		}
	}

	const SourcePackage *spkg=dynamic_cast<const SourcePackage *>(package);
	if(!rebuild && spkg && spkg->get_config().get_mtime()>mtime)
		mark_rebuild("Package options changed");
}

void Target::build_done()
{
	building=false;
	rebuild=false;
}
