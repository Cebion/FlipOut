/* $Id: tar.cpp 89 2009-05-07 06:08:09Z tdb $

This file is part of builder
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cstring>
#include <msp/fs/stat.h>
#include <msp/fs/utils.h>
#include <msp/io/file.h>
#include <msp/io/print.h>
#include "builder.h"
#include "sourcepackage.h"
#include "tar.h"
#include "tarball.h"

using namespace std;
using namespace Msp;

Tar::Tar(Builder &b, const TarBall &t):
	InternalAction(b),
	tarball(t)
{
	string basename=FS::basename(tarball.get_path());
	announce(tarball.get_package()->get_name(), "TAR ", basename);
	if(builder.get_verbose()>=2)
		IO::print("Create %s\n", basename);

	if(!builder.get_dry_run())
		worker=new Worker(*this);
}


Tar::Worker::Worker(Tar &t):
	tar(t)
{
	launch();
}

void Tar::Worker::main()
{
	const FS::Path &pkg_src=tar.tarball.get_package()->get_source();
	FS::Path basedir=FS::basepart(FS::basename(tar.tarball.get_path()));

	IO::File out(tar.tarball.get_path().str(), IO::M_WRITE);
	const TargetList &deps=tar.tarball.get_depends();
	for(TargetList::const_iterator i=deps.begin(); i!=deps.end(); ++i)
	{
		FileTarget *ft=dynamic_cast<FileTarget *>(*i);
		if(!ft)
			continue;

		char buf[4096];
		memset(buf, 0, 512);

		string rel_path=(basedir/relative(ft->get_path(), pkg_src)).str();
		if(rel_path.size()>99)
		{
			IO::print("Can't store %s in tar archive - too long name\n", rel_path);
			error=true;
			break;
		}

		memcpy(buf, rel_path.data(), rel_path.size());

		struct stat st=FS::stat(ft->get_path());
		store_number(buf+100, st.st_mode, 7);
		store_number(buf+108, st.st_uid, 7);
		store_number(buf+116, st.st_gid, 7);
		store_number(buf+124, st.st_size, 11);
		store_number(buf+136, st.st_mtime, 11);
		buf[156]='0';

		memset(buf+148, ' ', 8);
		unsigned chk=0;
		for(unsigned j=0; j<512; ++j)
			chk+=static_cast<unsigned char>(buf[j]);
		store_number(buf+148, chk, 7);
		buf[155]=0;

		out.write(buf, 512);
		IO::File in(ft->get_path().str());
		for(int j=0; j<st.st_size; j+=4096)
		{
			unsigned len=in.read(buf, 4096);
			len+=((~len)+1)&0777;
			out.write(buf, len);
		}
	}

	done=true;
}

void Tar::Worker::store_number(char *buf, unsigned value, unsigned length)
{
	for(unsigned i=length; i--;)
	{
		buf[i]='0'+value%8;
		value/=8;
	}
}
