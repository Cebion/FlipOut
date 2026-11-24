#include <windows.h>
#include <msp/core/mutex.h>
#include <msp/core/systemerror.h>
#include <msp/strings/regex.h>
#include "dir.h"

using namespace std;

namespace Msp {
namespace FS {

void mkdir(const Path &path, int)
{
	if(!CreateDirectory(path.str().c_str(), NULL))
		throw system_error("CreateDirectory");
}

void rmdir(const Path &path)
{
	if(!RemoveDirectory(path.str().c_str()))
		throw system_error("RemoveDirectory");
}

vector<string> list_filtered(const Path &path, const string &filter)
{
	Regex r_filter(filter);

	vector<string> result;
	WIN32_FIND_DATA entry;
	string pattern = path.str()+"\\*";
	HANDLE search_handle = FindFirstFileEx(pattern.c_str(), FindExInfoBasic, &entry, FindExSearchNameMatch, 0, 0);
	if(search_handle==INVALID_HANDLE_VALUE)
	{
		DWORD err = GetLastError();
		if(err==ERROR_FILE_NOT_FOUND)
			return result;
		throw system_error("FindFirstFileEx");
	}

	bool more = true;
	for(; more; more=FindNextFile(search_handle, &entry))
	{
		const char *fn = entry.cFileName;
		if(fn[0]=='.' && (fn[1]==0 || (fn[1]=='.' && fn[2]==0)))
			continue;
		if(r_filter.match(fn))
			result.push_back(fn);
	}

	DWORD err = GetLastError();
	FindClose(search_handle);
	if(err!=ERROR_NO_MORE_FILES)
		throw system_error("FindNextFile");

	return result;
}

/* Windows documentation says Get/SetCurrentDirectory use a global buffer and
are not thread safe. */
static Mutex &cwd_mutex()
{
	static Mutex mutex;
	return mutex;
}

Path getcwd()
{
	MutexLock lock(cwd_mutex());

	char buf[1024];
	Path result;
	DWORD len = GetCurrentDirectory(sizeof(buf), buf);
	if(len>=sizeof(buf))
	{
		vector<char> buf2(len+1);
		len = GetCurrentDirectory(buf2.size(), buf2.data());
		result = buf2.data();
	}
	else
		result = buf;

	if(!len)
		throw system_error("GetCurrentDirectory");

	return result;
}

void chdir(const Path &path)
{
	MutexLock lock(cwd_mutex());
	if(!SetCurrentDirectory(path.c_str()))
		throw system_error("SetCurrentDirectory");
}

} // namespace FS
} // namespace Msp
