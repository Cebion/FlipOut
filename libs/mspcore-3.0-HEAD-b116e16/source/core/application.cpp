#include <cstring>
#include <typeinfo>
#include <signal.h>
#include <msp/debug/demangle.h>
#include <msp/debug/errorreporter.h>
#include <msp/fs/dir.h>
#include <msp/fs/path.h>
#include <msp/fs/utils.h>
#include <msp/io/print.h>
#include <msp/strings/utils.h>
#include "application.h"
#include "getopt.h"

using namespace std;

namespace Msp {

Application *Application::app_ = 0;
Application::Starter *Application::starter_ = 0;
const char *Application::argv0_ = 0;
string Application::name_;
void *Application::data_ = 0;

Application::Application(const string &n):
	exit_code(0)
{
	if(app_)
		throw logic_error("instance already exists");

	if(!n.empty())
		name_ = n;
	else
		name_ = FS::basename(argv0_);
}

int Application::run(int argc, char **argv, void *data, void (*created_callback)(void *))
{
	if(!starter_)
	{
		IO::cerr.write("Application::run called with no RegisteredApplication class!\n");
		return 126;
	}

	set_startup_info(argv[0], data);

	try
	{
		try
		{
			app_ = starter_->create_app(argc, argv);
		}
		catch(const usage_error &e)
		{
			IO::print(IO::cerr, "%s\n%s\n", e.what(), e.help());
			return 1;
		}

		if(created_callback)
			created_callback(data);

		int result = app_->main();
		Application *a = app_;
		app_ = 0;
		delete a;
		return result;
	}
	catch(const exception &e)
	{
		bool handled = false;
		if(const Debug::ErrorReporter *er = Debug::ErrorReporter::get_current())
			handled = er->report_uncaught_exception(e);

		if(!handled)
		{
			IO::print(IO::cerr, "An uncaught exception occurred.\n");
			IO::print(IO::cerr, "  type:   %s\n", Debug::demangle(typeid(e).name()));
			vector<string> lines = split(e.what(), '\n');
			if(lines.size()<2)
				IO::print(IO::cerr, "  what(): %s\n", e.what());
			else
			{
				IO::print(IO::cerr, "  what(): %s\n", lines.front());
				for(auto i=lines.begin(); ++i!=lines.end(); )
					IO::print(IO::cerr, "          %s\n", *i);
			}
		}

		delete app_;
		app_ = 0;

		return 124;
	}
}

void Application::set_startup_info(const char *argv0, void *data)
{
	if(argv0_)
		throw logic_error("startup info already set");

	static FS::Path exe;

	if(!argv0 || !*argv0)
	{
#ifdef _WIN32
		argv0 = "application.exe";
#else
		argv0 = "./application";
#endif
	}

	bool has_slash = strchr(argv0, FS::DIRSEP);
	if(!has_slash)
		exe = FS::path_lookup(argv0);
	if(exe.empty())
		exe = FS::realpath(argv0);

	argv0_ = exe.c_str();
	data_ = data;
}

int Application::main()
{
	done = false;
	while(!done)
		tick();

	return exit_code;
}

void Application::catch_signal(int s)
{
	signal(s, &sighandler_);
}

void Application::exit(int c)
{
	done = true;
	exit_code = c;
}

void Application::sighandler_(int s)
{
	app_->sighandler(s);
}


Application::Starter::Starter()
{
	if(starter_)
		throw logic_error("Can't create more than one Starter instance");

	starter_ = this;
}

} // namespace Msp
