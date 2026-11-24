/* $Id: externalaction.cpp 89 2009-05-07 06:08:09Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cstring>
#include <cstdlib>
#include <sys/wait.h>
#include <msp/fs/dir.h>
#include <msp/io/print.h>
#include <msp/strings/utils.h>
#include "builder.h"
#include "externalaction.h"

using namespace std;
using namespace Msp;

void ExternalAction::launch()
{
	if(builder.get_verbose()>=2)
		IO::print("%s\n", join(argv.begin(), argv.end()));

	if(builder.get_dry_run())
		pid=-1;
	else
	{
		pid=fork();
		if(pid==0)
		{
			char *argv_[argv.size()+1];

			unsigned j=0;
			for(StringList::iterator i=argv.begin(); i!=argv.end(); ++i)
				argv_[j++]=strdup(i->c_str());
			argv_[j]=0;

			if(!work_dir.empty())
				FS::chdir(work_dir);
			execvp(argv_[0], argv_);
			IO::print("Couldn't execute %s\n", argv.front());
			exit(1);
		}
		else if(pid<0)
			pid=0;
	}
}

int ExternalAction::check()
{
	if(builder.get_dry_run())
	{
		signal_done.emit();
		return 0;
	}

	if(!pid)
		return exit_code;

	int status;
	if(waitpid(pid, &status, WNOHANG)==pid)
	{
		signal_done.emit();
		if(WIFEXITED(status))
			exit_code=WEXITSTATUS(status);
		else
			exit_code=254;
		pid=0;
		return exit_code;
	}
	else
		return -1;
}
