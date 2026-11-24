/* $Id: install.cpp 102 2009-09-21 16:23:33Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/fs/utils.h>
#include "builder.h"
#include "copy.h"
#include "executable.h"
#include "datafile.h"
#include "header.h"
#include "install.h"
#include "package.h"
#include "pkgconfig.h"
#include "sharedlibrary.h"
#include "staticlibrary.h"

using namespace std;
using namespace Msp;

Install::Install(Builder &b, const SourcePackage &p, FileTarget &s, const std::string &loc):
	FileTarget(b, &p, generate_target_path(s, loc)),
	source(s)
{
	buildable=true;
	add_depend(&source);
}

void Install::check_rebuild()
{
	if(!mtime)
		mark_rebuild("Does not exist");
	else if(source.get_mtime()>mtime || source.get_size()!=size)
		mark_rebuild(FS::basename(source.get_name())+" has changed");
	else if(source.get_rebuild())
		mark_rebuild(FS::basename(source.get_name())+" needs rebuilding");
}

Action *Install::create_action()
{
	return new Copy(builder, *package, source.get_path(), path);
}

FS::Path Install::generate_target_path(const FileTarget &tgt, const std::string &loc)
{
	if(!tgt.get_package())
		throw InvalidParameterValue("Can't install package-less targets");

	FS::Path base=tgt.get_package()->get_builder().get_prefix();
	string tgtname=FS::basename(tgt.get_path());

	string mid;
	if(!loc.empty())
		mid=loc;
	else if(const Header *hdr=dynamic_cast<const Header *>(&tgt))
	{
		if(hdr->get_component()->get_type()!=Component::HEADERS)
			throw Exception("Header install from non-header component?");
		mid="include/"+hdr->get_component()->get_name();
	}
	else if(dynamic_cast<const Executable *>(&tgt))
		mid="bin";
	else if(const SharedLibrary *shlib=dynamic_cast<const SharedLibrary *>(&tgt))
	{
		const Component &comp=shlib->get_component();
		if(comp.get_type()==Component::LIBRARY)
			mid="lib";
		else if(comp.get_type()==Component::MODULE)
			mid="lib/"+tgt.get_package()->get_name();
	}
	else if(dynamic_cast<const StaticLibrary *>(&tgt))
		mid="lib";
	else if(dynamic_cast<const PkgConfig *>(&tgt))
		mid="lib/pkgconfig";
	else if(dynamic_cast<const ::DataFile *>(&tgt))
		mid="share/"+tgt.get_package()->get_name();

	if(mid.empty())
		throw InvalidParameterValue("Don't know where to install "+tgtname);

	return (base/mid/tgtname).str();
}
