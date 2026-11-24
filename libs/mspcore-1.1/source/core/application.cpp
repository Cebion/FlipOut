/* $Id: application.cpp 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore
Copyright © 2006 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <signal.h>
#include <iostream>
#include <typeinfo>
#include "../debug/backtrace.h"
#include "../debug/demangle.h"
#include "../time/units.h"
#include "../time/utils.h"
#include "application.h"
#include "except.h"

using namespace std;

namespace Msp {

Application::Application():
	exit_code(0),
	loop_mode_(TICK_SLEEP)
{ }

/**
Constructs an instance of the registered application class and runs it.  If the
application throws a UsageError, the static usage() function is called.

This function can only be called once.  The global main() function provided by
the library normally does it automatically at program startup.
*/
int Application::run(int argc, char **argv, void *data)
{
	static bool called=false;
	if(called)
	{
		cerr<<"Trying to call Application::run_app twice!\n";
		return 125;
	}
	called=true;

	if(!reg_app_)
	{
		cerr<<"Trying to run with no application class registered!\n";
		return 126;
	}

	data_=data;

	try
	{
		try
		{
			app_=reg_app_->create_app(argc, argv);
		}
		catch(const UsageError &e)
		{
			reg_app_->usage(e.what(), argv[0], e.get_brief());
			return 1;
		}

		int result=app_->main();
		delete app_;
		return result;
	}
	catch(const exception &e)
	{
		delete app_;

#ifdef WIN32
		string msg=Debug::demangle(typeid(e).name())+":\n"+e.what();
		MessageBoxA(0, msg.c_str(), "Uncaught exception", MB_OK|MB_ICONERROR);
#else
		cerr<<"An uncaught exception occurred.\n";
		cerr<<"  type:   "<<Debug::demangle(typeid(e).name())<<'\n';
		cerr<<"  what(): "<<e.what()<<'\n';

		const Exception *exc=dynamic_cast<const Exception *>(&e);
		if(exc && !exc->get_backtrace().get_frames().empty())
		{
			cerr<<"  backtrace:\n";
			const list<Debug::Backtrace::StackFrame> &frames=exc->get_backtrace().get_frames();
			for(list<Debug::Backtrace::StackFrame>::const_iterator i=frames.begin(); i!=frames.end(); ++i)
				cerr<<"    "<<*i<<'\n';
		}
#endif

		return 124;
	}
}

/**
Prints a message describing the usage of the application.  The default version
will blame the programmer for being lazy.

@param   reason  Why the function was called
@param   argv0   The value of argv[0], to be used in the message
@param   brief   Whether to print a brief or long usage message
*/
void Application::usage(const char *reason, const char *, bool)
{
	if(reason)
		cerr<<"UsageError: "<<reason<<'\n';
	cerr<<"The programmer was lazy and didn't write a usage() function for this application.\n";
}

/**
Default main loop.  Behavior depends on loop_mode_.  A custom main loop should
monitor the done member variable and return exit_code.
*/
int Application::main()
{
	if(loop_mode_==NONE)
		return 0;

	done=false;
	while(!done)
	{
		if(loop_mode_==SLEEP)
		{
			sleep_sem_.wait();
			if(!done)
				tick();
		}
		else if(loop_mode_==TICK_SLEEP)
		{
			tick();
			Time::sleep(Time::msec);
		}
		else if(loop_mode_==TICK_YIELD)
		{
			tick();
#ifdef WIN32
			Sleep(0);
#else
			sched_yield();
#endif
		}
		else if(loop_mode_==TICK_BUSY)
			tick();
	}

	return exit_code;
}

/**
Sets the specified signal to be delivered to the sighandler member function.
*/
void Application::catch_signal(int s)
{
	signal(s, &sighandler_);
}

/**
Changes the main loop mode.
*/
void Application::set_loop_mode(LoopMode l)
{
	LoopMode old_mode=loop_mode_;
	loop_mode_=l;
	if(old_mode==SLEEP)
		sleep_sem_.signal();
}

/**
Causes the tick() function to be executed once if loop mode is SLEEP.  Has no
effect with other loop modes.
*/
void Application::induce_tick()
{
	if(loop_mode_==SLEEP)
		sleep_sem_.signal();
}

/**
Causes the application to exit gracefully with the given exit code.
*/
void Application::exit(int c)
{
	done=true;
	exit_code=c;
	if(loop_mode_==SLEEP)
		sleep_sem_.signal();
}

/**
Static wrapper function to call a member function of the Application instance.
*/
void Application::sighandler_(int s)
{
	app_->sighandler(s);
}


Application::RegBase::RegBase()
{
	if(reg_app_)
	{
		cerr<<"Warning: registering the application twice\n";
		delete reg_app_;
	}

	reg_app_=this;
}

Application *Application::app_=0;
Application::RegBase *Application::reg_app_=0;
void *Application::data_=0;

} // namespace Msp
