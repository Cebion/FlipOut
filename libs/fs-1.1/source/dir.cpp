/* $Id$

This file is part of libmspfs
Copyright © 2006-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cstdlib>
#include <cerrno>
#include <dirent.h>
#include <sys/stat.h>
#ifdef WIN32
#include <shlobj.h>
#endif
#include <msp/core/except.h>
#include <msp/strings/regex.h>
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

/**
Helper function to determine the location of the program's executable.  Caches
the last result to cut down filesystem access with repeated calls.
*/
const Path &get_bin_dir(const string &argv0)
{
	static string last_argv0;
	static Path bin_dir;

	if(!(argv0==last_argv0))
	{
		Path exe;
		if(argv0.find('/')==string::npos)
		{
			const char *path=getenv("PATH");
			vector<string> dirs=split(path, ':');
			for(vector<string>::const_iterator i=dirs.begin(); i!=dirs.end(); ++i)
				if(exists(Path(*i)/argv0))
				{
					exe=realpath(Path(*i)/argv0);
					break;
				}
		}
		else
			exe=realpath(argv0);

		last_argv0=argv0;
		bin_dir=dirname(exe);
	}

	return bin_dir;
}

}

void mkdir(const Path &path, int mode)
{
	int err;
#ifdef WIN32
	// The win32 version of this function doesn't take the mode argument.  Go figure.
	(void)mode;
	err=::mkdir(path.str().c_str());
#else
	err=::mkdir(path.str().c_str(), mode);
#endif

	if(err==-1)
		throw SystemError("mkdir failed", errno);
}

void mkpath(const Path &path, int mode)
{
	Path p;
	for(Path::Iterator i=path.begin(); i!=path.end(); ++i)
	{
		p/=*i;
#ifdef WIN32
		if(p.size()==1 && p.is_absolute())
			continue;
#endif
		struct stat st;
		int err=stat(p, st);
		if(err==0)
		{
			if(!S_ISDIR(st.st_mode))
				throw Exception("A component exists and is not a directory");
			continue;
		}
		else if(errno!=ENOENT)
			throw SystemError("stat failed", errno);
		else
			mkdir(p, mode);
	}
}

void rmdir(const Path &path)
{
	if(::rmdir(path.str().c_str())==-1)
		throw SystemError("rmdir failed", errno);
}

void rmdirs(const Path &path)
{
	list<string> files=list_files(path);
	for(list<string>::iterator i=files.begin(); i!=files.end(); ++i)
	{
		Path p=path / *i;
		struct stat st=stat(p.str().c_str());
		if(S_ISDIR(st.st_mode))
			rmdirs(p);
		else
			unlink(p);
	}

	rmdir(path);
}

list<string> list_files(const Path &path)
{
	return list_filtered(path, string());
}

list<string> list_filtered(const Path &path, const string &filter)
{
	Regex r_filter(filter);

	list<string> result;
	DIR *dir=opendir(path.str().c_str());
	if(dir)
	{
		while(dirent *de=readdir(dir))
		{
			const char *fn=de->d_name;
			if(fn[0]=='.' && (fn[1]==0 || (fn[1]=='.' && fn[2]==0))) 
				continue;
			if(r_filter.match(fn))
				result.push_back(fn);
		}
		closedir(dir);
	}

	return result;
}

Path getcwd()
{
	char buf[1024];
	return ::getcwd(buf, sizeof(buf));
}

Path get_home_dir()
{
#ifdef WIN32
	char home[MAX_PATH];
	if(SHGetFolderPath(0, CSIDL_PERSONAL, 0, 0, home)==S_OK)
		return home;
#else
	const char *home=getenv("HOME");
	if(home)
		return home;
#endif
	return ".";
}

Path get_user_data_dir(const string &appname)
{
#ifdef WIN32
	char datadir[MAX_PATH];
	if(SHGetFolderPath(0, CSIDL_LOCAL_APPDATA, 0, 0, datadir)==S_OK)
		return Path(datadir)/appname;
	return ".";
#else
	return get_home_dir()/("."+appname);
#endif
}

Path get_sys_conf_dir(const string &argv0)
{
	Path dir=get_bin_dir(argv0);

	if(dir[-1]=="bin" || dir[-1]=="sbin")
	{
		dir/="..";
		if(dir[-1]=="usr")
			dir/="..";
		return dir/"etc";
	}
	else
		return dir;
}

Path get_sys_data_dir(const string &argv0, const string &appname)
{
	Path dir=get_bin_dir(argv0);

	if(dir[-1]=="bin" || dir[-1]=="sbin")
		return dir/".."/"share"/appname;
	else
		return dir;
}

void chdir(const Path &path)
{
	if(::chdir(path.str().c_str())==-1)
		throw SystemError("chdir failed", errno);
}

} // namespace FS
} // namespace Msp
