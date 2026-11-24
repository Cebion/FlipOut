/* $Id: binarypackage.cpp 92 2009-05-13 20:15:38Z tdb $

This file is part of builder
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/io/print.h>
#include <msp/strings/utils.h>
#include "binarypackage.h"
#include "builder.h"

using namespace std;
using namespace Msp;

BinaryPackage::BinaryPackage(Builder &b, const string &n):
	Package(b, n),
	need_path(false)
{
	use_pkgconfig=false;
}

void BinaryPackage::set_path(const FS::Path &p)
{
	path=builder.get_cwd()/p;
}

void BinaryPackage::create_build_info()
{
	for(StringList::iterator i=export_binfo.incpath.begin(); i!=export_binfo.incpath.end(); ++i)
		if((*i)[0]=='@')
			*i=(path/i->substr(1)).str();

	for(StringList::iterator i=export_binfo.libpath.begin(); i!=export_binfo.libpath.end(); ++i)
		if((*i)[0]=='@')
			*i=(path/i->substr(1)).str();
}

BinaryPackage *BinaryPackage::from_pkgconfig(Builder &builder, const string &name)
{
	string info=builder.run_pkgconfig(name, "flags");

	BinaryPackage *pkg=new BinaryPackage(builder, name);
	pkg->use_pkgconfig=true;
	BuildInfo &binfo=pkg->export_binfo;

	vector<string> flags=split(info);
	for(vector<string>::const_iterator i=flags.begin(); i!=flags.end(); ++i)
	{
		if(!i->compare(0, 2, "-I"))
			binfo.incpath.push_back(i->substr(2));
		else if(!i->compare(0, 2, "-D"))
			binfo.defines.push_back(i->substr(2));
		else if(!i->compare(0, 2, "-L"))
			binfo.libpath.push_back(i->substr(2));
		else if(!i->compare(0, 2, "-l"))
			binfo.libs.push_back(i->substr(2));
	}

	return pkg;
}


BinaryPackage::Loader::Loader(BinaryPackage &p):
	Package::Loader(p)
{
	add("need_path", &BinaryPackage::need_path);
	add("build_info", &Loader::build_info);
}

void BinaryPackage::Loader::build_info()
{
	load_sub(static_cast<BinaryPackage &>(pkg).export_binfo);
}
