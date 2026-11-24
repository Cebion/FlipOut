#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <msp/core/systemerror.h>
#include <msp/fs/dir.h>
#include <msp/io/console.h>
#include "process.h"
#include "process_private.h"

using namespace std;

namespace Msp {

Process::~Process()
{ }

void Process::platform_get_self_info(Private &priv)
{
	priv.info.pid = getpid();
}

void Process::execute(const string &command, bool path_search, const Arguments &args)
{
	pid_t pid = 0;
	if(this!=_self)
		pid = fork();

	if(pid==-1)
		throw system_error("fork");
	else if(pid==0)
	{
		try
		{
			vector<const char *> argv(args.size()+2);
			argv[0] = command.c_str();
			for(unsigned i=0; i<args.size(); ++i)
				argv[i+1] = args[i].c_str();
			argv[args.size()+1] = 0;

			if(redirect)
			{
				// dup2 clears O_CLOEXEC
				if(cin)
					IO::cin.redirect(*cin);
				if(cout)
					IO::cout.redirect(*cout);
				if(cerr)
					IO::cerr.redirect(*cerr);
			}

			if(!work_dir.empty())
				FS::chdir(work_dir);

			if(path_search)
				execvp(command.c_str(), const_cast<char *const *>(&argv[0]));
			else
				execv(command.c_str(), const_cast<char *const *>(&argv[0]));
		}
		catch(...)
		{ }
		_exit(255);
	}
	else
	{
		priv->info.pid = pid;
		running = true;
	}
}

bool Process::wait(bool block)
{
	if(!running)
		throw logic_error("not running");

	int status;
	int pid = waitpid(priv->info.pid, &status, (block ? 0 : WNOHANG));
	if(pid==-1)
		throw system_error("waitpid");

	if(pid)
	{
		finished = true;
		running = false;
		if(WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		else if(WIFSIGNALED(status))
			exit_code = 0x100|WTERMSIG(status);
	}

	return finished;
}

void Process::terminate()
{
	::kill(priv->info.pid, SIGTERM);
}

void Process::kill()
{
	::kill(priv->info.pid, SIGKILL);
}

void Process::interrupt()
{
	::kill(priv->info.pid, SIGINT);
}


Process::Private::Private()
{
	info.pid = 0;
}

} // namespace Msp
