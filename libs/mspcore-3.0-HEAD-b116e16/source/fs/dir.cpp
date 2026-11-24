#include <msp/core/application.h>
#include <msp/core/environ.h>
#include <msp/strings/utils.h>
#include "dir.h"
#include "path.h"
#include "stat.h"
#include "utils.h"

using namespace std;

namespace Msp {
namespace FS {

namespace
{

enum
{
#ifdef _WIN32
	ITEMSEP = ';'
#else
	ITEMSEP = ':'
#endif
};

/** Helper function to determine the location of the program's executable.
Caches the last result to cut down filesystem access with repeated calls. */
const Path &get_bin_dir(const string &argv0)
{
	static string last_argv0;
	static Path bin_dir;

	if(!(argv0==last_argv0))
	{
		Path exe;
		if(argv0.find(DIRSEP)==string::npos)
		{
			string path = getenv("PATH");
			if(!path.empty())
			{
				for(const string &d: split(path, ITEMSEP))
					if(exists(Path(d)/argv0))
					{
						exe = realpath(Path(d)/argv0);
						break;
					}
			}
		}

		if(exe.empty())
			exe = realpath(argv0);

		last_argv0 = argv0;
		bin_dir = dirname(exe);
	}

	return bin_dir;
}

}


not_a_directory::not_a_directory(const Path &p):
	runtime_error(p.str())
{ }


void mkpath(const Path &path, int mode)
{
	Path p;
	for(const string &c: path)
	{
		p /= c;
#ifdef _WIN32
		if(p.size()==1 && p.is_absolute())
			continue;
#endif
		if(FS::Stat st = stat(p))
		{
			if(!st.is_directory())
				throw not_a_directory(p);
			continue;
		}
		else
			mkdir(p, mode);
	}
}

void rmpath(const Path &path)
{
	for(const string &f: list_files(path))
	{
		Path p = path/f;
		if(is_dir(p))
			rmpath(p);
		else
			unlink(p);
	}

	rmdir(path);
}

vector<string> list_files(const Path &path)
{
	return list_filtered(path, string());
}

Path get_sys_conf_dir()
{
	const char *argv0 = Application::get_argv0();
	if(!argv0)
		throw logic_error("no startup command");

	Path dir = get_bin_dir(argv0);

	if(dir[-1]=="bin" || dir[-1]=="sbin")
	{
		dir /= "..";
		if(dir[-1]=="usr")
			dir /= "..";
		return dir/"etc";
	}
	else
		return dir;
}

Path get_sys_data_dir()
{
	const char *argv0 = Application::get_argv0();
	if(!argv0)
		throw logic_error("no startup command");

	Path dir = get_bin_dir(argv0);

	if(dir[-1]=="bin" || dir[-1]=="sbin")
		return dir/".."/"share"/Application::get_name();
	else if(dir[-1]=="MacOS")
		return dir/".."/"Resources";
	else
		return dir;
}

Path get_sys_lib_dir()
{
	const char *argv0 = Application::get_argv0();
	if(!argv0)
		throw logic_error("no startup command");

	Path dir = get_bin_dir(argv0);

	if(dir[-1]=="bin" || dir[-1]=="sbin")
		return dir/".."/"lib"/Application::get_name();
	else
		return dir;
}

Path path_lookup(const string &name, const vector<Path> &paths)
{
	for(const Path &p: paths)
	{
		Path full = p/name;
		if(exists(full))
			return realpath(full);
	}

	return Path();
}

Path path_lookup(const string &name)
{
	string path = getenv("PATH");
	vector<string> dirs = split(path, ITEMSEP);
	return path_lookup(name, vector<Path>(dirs.begin(), dirs.end()));
}

} // namespace FS
} // namespace Msp
