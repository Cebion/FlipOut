/* $Id: misc.cpp 91 2009-05-11 20:43:21Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <sys/wait.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>
#include <msp/io/print.h>
#include "misc.h"

using namespace std;
using namespace Msp;

string run_command(const StringList &argv, int *status)
{
	int pfd[2];
	pipe(pfd);

	string result;

	pid_t pid=fork();
	if(pid==0)
	{
		char *argv_[argv.size()+1];

		unsigned j=0;
		for(StringList::const_iterator i=argv.begin(); i!=argv.end(); ++i)
			argv_[j++]=strdup(i->c_str());
		argv_[j]=0;

		close(pfd[0]);
		dup2(pfd[1], 1);
		close(pfd[1]);
		int devnull=open("/dev/null", O_WRONLY);
		dup2(devnull, 2);
		close(devnull);

		execvp(argv_[0], argv_);
		::exit(1);
	}
	else if(pid==-1)
		IO::print(IO::cerr, "Failed to execute %s\n", argv.front());
	else
	{
		close(pfd[1]);
		while(1)
		{
			char buf[1024];
			int len=read(pfd[0], buf, sizeof(buf));
			if(len<=0)
			{
				int s;
				if(waitpid(pid, &s, WNOHANG))
				{
					if(status)
					{
						if(WIFEXITED(s))
							*status=WEXITSTATUS(s);
						else
							*status=-1;
					}
					break;
				}
			}
			else
				result.append(buf, len);
		}
		close(pfd[0]);
	}

	return result;
}


