#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <msp/core/systemerror.h>
#include <msp/strings/regex.h>
#include "dir.h"

using namespace std;

namespace Msp {
namespace FS {

void mkdir(const Path &path, int mode)
{
	if(::mkdir(path.str().c_str(), mode)==-1)
		throw system_error("mkdir");
}

void rmdir(const Path &path)
{
	if(::rmdir(path.str().c_str())==-1)
		throw system_error("rmdir");
}

vector<string> list_filtered(const Path &path, const string &filter)
{
	Regex r_filter(filter);

	vector<string> result;
	DIR *dir = opendir(path.str().c_str());
	if(!dir)
		throw system_error("opendir");

	while(dirent *de = readdir(dir))
	{
		const char *fn = de->d_name;
		if(fn[0]=='.' && (fn[1]==0 || (fn[1]=='.' && fn[2]==0)))
			continue;
		if(r_filter.match(fn))
			result.push_back(fn);
	}
	closedir(dir);

	return result;
}

Path getcwd()
{
	char buf[1024];
	return ::getcwd(buf, sizeof(buf));
}

void chdir(const Path &path)
{
	if(::chdir(path.str().c_str())==-1)
		throw system_error("chdir");
}

} // namespace FS
} // namespace Msp
