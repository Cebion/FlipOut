/* $Id: copy.cpp 101 2009-09-21 16:19:49Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <errno.h>
#include <msp/fs/dir.h>
#include <msp/fs/stat.h>
#include <msp/fs/utils.h>
#include <msp/io/file.h>
#include <msp/io/print.h>
#include "builder.h"
#include "copy.h"
#include "package.h"

using namespace std;
using namespace Msp;

Copy::Copy(Builder &b, const Package &pkg, const FS::Path &s, const FS::Path &d):
	InternalAction(b),
	src(s),
	dest(d)
{
	announce(pkg.get_name(), "COPY", dest.str());
	if(builder.get_verbose()>=2)
		IO::print("%s -> %s\n", s, d);

	if(!builder.get_dry_run())
		worker=new Worker(*this);
}


Copy::Worker::Worker(Copy &c):
	copy(c)
{
	launch();
}

void Copy::Worker::main()
{
	FS::mkpath(FS::dirname(copy.dest), 0755);

	// Remove old file.  Not doing this would cause Bad Stuff when installing libraries.
	if(FS::exists(copy.dest))
	{
		try
		{
			unlink(copy.dest);
		}
		catch(const Exception &e)
		{
			IO::print(IO::cerr, "%s\n", e.what());
			done=error=true;
			return;
		}
	}

	try
	{
		IO::File in(copy.src.str());
		IO::File out(copy.dest.str(), IO::M_WRITE);

		// Actual transfer loop
		char buf[16384];
		while(!in.eof())
		{
			unsigned len=in.read(buf, sizeof(buf));
			out.write(buf, len);
		}
	}
	catch(const Exception &e)
	{
		IO::print(IO::cerr, "%s\n", e.what());
		done=error=true;
		return;
	}

	// Preserve file permissions
	struct stat st=FS::stat(copy.src);
	chmod(copy.dest.str().c_str(), st.st_mode&0777);

	done=true;
}
