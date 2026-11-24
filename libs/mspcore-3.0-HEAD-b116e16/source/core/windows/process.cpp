#include <windows.h>
#include <msp/core/systemerror.h>
#include <msp/io/handle_private.h>
#include <msp/strings/utils.h>
#include "process.h"
#include "process_private.h"

using namespace std;

namespace {

string quote_argument(const string &arg)
{
	string result;
	bool need_quotes = false;
	bool backslash = false;
	for(char c: arg)
	{
		if(c=='\\')
			backslash = true;
		else if(c=='"')
		{
			if(backslash)
				result += '\\';
			result += '\\';
		}
		else if(c==' ')
			need_quotes = true;
		result += c;
	}

	if(need_quotes)
		return "\""+result+"\"";
	else
		return result;
}

}

namespace Msp {

Process::~Process()
{
	CloseHandle(priv->info.hProcess);
	CloseHandle(priv->info.hThread);
}

void Process::platform_get_self_info(Private &priv)
{
	priv.info.hProcess = GetCurrentProcess();
	priv.info.hThread = 0;
	priv.info.dwProcessId = GetCurrentProcessId();
	priv.info.dwThreadId = 0;
}

void Process::execute(const string &command, bool path_search, const Arguments &args)
{
	string cmdline = quote_argument(command);
	for(const string &a: args)
		append(cmdline, " ", quote_argument(a));

	STARTUPINFO startup;
	startup.cb = sizeof(STARTUPINFO);
	startup.lpReserved = 0;
	startup.lpDesktop = 0;
	startup.lpTitle = 0;
	startup.dwFlags = 0;
	startup.cbReserved2 = 0;
	startup.lpReserved2 = 0;
	if(redirect)
	{
		startup.dwFlags |= STARTF_USESTDHANDLES;
		HANDLE self_handle = self().priv->info.hProcess;
		HANDLE cin_handle = (cin ? *cin->get_handle(IO::M_READ) : GetStdHandle(STD_INPUT_HANDLE));
		DuplicateHandle(self_handle, cin_handle, self_handle, &startup.hStdInput, 0, true, DUPLICATE_SAME_ACCESS);
		HANDLE cout_handle = (cout ? *cout->get_handle(IO::M_WRITE) : GetStdHandle(STD_OUTPUT_HANDLE));
		DuplicateHandle(self_handle, cout_handle, self_handle, &startup.hStdOutput, 0, true, DUPLICATE_SAME_ACCESS);
		HANDLE cerr_handle = (cerr ? *cerr->get_handle(IO::M_WRITE) : GetStdHandle(STD_ERROR_HANDLE));
		DuplicateHandle(self_handle, cerr_handle, self_handle, &startup.hStdError, 0, true, DUPLICATE_SAME_ACCESS);
	}
	const char *cmdptr = (path_search ? 0 : command.c_str());
	const char *wd = (work_dir.empty() ? 0 : work_dir.c_str());
	if(!CreateProcess(cmdptr, const_cast<char *>(cmdline.c_str()), 0, 0, true, 0, 0, wd, &startup, &priv->info))
		throw system_error("CreateProcess");

	if(redirect)
	{
		CloseHandle(startup.hStdInput);
		CloseHandle(startup.hStdOutput);
		CloseHandle(startup.hStdError);
	}

	running = true;

	if(this==_self)
		TerminateProcess(priv->info.hProcess, 0);
}

bool Process::wait(bool block)
{
	if(!running)
		throw logic_error("not running");

	DWORD ret = WaitForSingleObject(priv->info.hProcess, (block ? INFINITE : 0));
	if(ret==WAIT_FAILED)
		throw system_error("WaitForSingleObject");

	if(ret==WAIT_OBJECT_0)
	{
		running = false;
		finished = true;
		DWORD ec;
		if(GetExitCodeProcess(priv->info.hProcess, &ec))
			exit_code = ec;
	}

	return finished;
}

void Process::terminate()
{
	TerminateProcess(priv->info.hProcess, 1);
}

void Process::kill()
{
	TerminateProcess(priv->info.hProcess, 1);
}

void Process::interrupt()
{
	TerminateProcess(priv->info.hProcess, 1);
}


Process::Private::Private()
{
	info.hProcess = 0;
	info.hThread = 0;
	info.dwProcessId = 0;
	info.dwThreadId = 0;
}

} // namespace Msp
