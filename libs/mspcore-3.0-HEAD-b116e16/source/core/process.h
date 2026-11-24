#ifndef MSP_CORE_PROCESS_H_
#define MSP_CORE_PROCESS_H_

#include <string>
#include <vector>
#include <msp/fs/path.h>
#include <msp/io/base.h>
#include "noncopyable.h"

namespace Msp {

/**
Provides an interface for running and controlling programs.

The creation of a new system-level process does not happen immediately when a
Process object is created, but only when exexute is called.  If called on the
object obtained with self(), the process is replaced with a new program
immediately.  In particular, destructors are not invoked.

Output redirections can be specified before calling execute.  To feed input to
the process or capture its output, use an IO::Pipe.  Redirections performed on
the self object take effect immediately.  It is recommended to perform such
redirections directly on the Console objects.
*/
class Process: private NonCopyable
{
public:
	typedef std::vector<std::string> Arguments;

private:
	struct Private;

	Private *priv;
	FS::Path work_dir;
	bool redirect;
	IO::Base *cin;
	IO::Base *cout;
	IO::Base *cerr;
	bool running;
	bool finished;
	unsigned exit_code;

	static Process *_self;

	Process(const Private &);
	void init();
public:
	Process();
	~Process();

	/** Returns an object referring to the current process. */
	static Process &self();
private:
	static void platform_get_self_info(Private &);

public:
	/** Sets the working directory for the new process.  By default, the working
	directory is not changed. */
	void set_working_directory(const FS::Path &);

	/** Redirects console input from an I/O object. */
	void redirect_cin(IO::Base &);

	/** Redirects console output to an I/O object. */
	void redirect_cout(IO::Base &);

	/** Redirects error output to an I/O object. */
	void redirect_cerr(IO::Base &);
private:
	void do_redirect(IO::Base *&, IO::Base &);

public:
	/** Executes a command, searching for it in the standard locations. */
	void execute(const std::string &, const Arguments &);

	/** Executes a command specified by a full path. */
	void execute(const FS::Path &, const Arguments &);

private:
	void execute(const std::string &, bool, const Arguments &);

public:
	bool is_running() const { return running; }
	bool has_finished() const { return finished; }

	/** Checks the status of a running process.  If block is true, waits for the
	process to finish. */
	bool wait(bool block = true);

	/** Returns the exit code for a finished process. */
	unsigned get_exit_code() const;

	/** Terminates the process. */
	void terminate();

	/** Brutally murders the process.  It is not given a chance to terminate
	gracefully. */
	void kill();

	/** Sends the process an interrupt signal, as if ^C was pressed. */
	void interrupt();
};

} // namespace Msp

#endif
