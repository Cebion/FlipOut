#include <msp/io/console.h>
#include "process.h"
#include "process_private.h"

using namespace std;

namespace Msp {

Process *Process::_self = 0;

Process::Process(const Private &p):
	priv(new Private(p))
{
	init();
}

Process::Process():
	priv(new Private)
{
	init();
}

void Process::init()
{
	redirect = false;
	cin = 0;
	cout = 0;
	cerr = 0;
	running = false;
	finished = false;
	exit_code = 0;
}

Process &Process::self()
{
	if(!_self)
	{
		Private _priv;
		platform_get_self_info(_priv);
		_self = new Process(_priv);
	}
	return *_self;
}

void Process::set_working_directory(const FS::Path &d)
{
	work_dir = d;
}

void Process::redirect_cin(IO::Base &io)
{
	do_redirect(cin, io);
}

void Process::redirect_cout(IO::Base &io)
{
	do_redirect(cout, io);
}

void Process::redirect_cerr(IO::Base &io)
{
	do_redirect(cerr, io);
}

void Process::do_redirect(IO::Base *&ptr, IO::Base &io)
{
	if(this==_self)
	{
		if(&ptr==&cin)
			IO::cin.redirect(io);
		else if(&ptr==&cout)
			IO::cout.redirect(io);
		else if(&ptr==&cerr)
			IO::cerr.redirect(io);
	}
	else
	{
		redirect = true;
		ptr = &io;
	}
}

void Process::execute(const string &command, const Arguments &args)
{
	execute(command, true, args);
}

void Process::execute(const FS::Path &command, const Arguments &args)
{
	execute(command.str(), false, args);
}

unsigned Process::get_exit_code() const
{
	if(!finished)
		throw logic_error("not finished");
	return exit_code;
}

} // namespace Msp
