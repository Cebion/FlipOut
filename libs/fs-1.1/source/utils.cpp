/* $Id$

This file is part of libmspfs
Copyright © 2006-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cerrno>
#include <msp/core/except.h>
#ifndef WIN32
#include <fnmatch.h>
#else
#include <msp/strings/glob.h>
#endif
#include <msp/strings/utils.h>
#include "dir.h"
#include "path.h"
#include "stat.h"
#include "utils.h"

using namespace std;

namespace Msp {
namespace FS {

string basename(const Path &p)
{
	return p[-1];
}

Path dirname(const Path &p)
{
	if(p.size()==1)
	{
		if(p.is_absolute())
			return p;
		return ".";
	}
	return p.subpath(0, p.size()-1);
}

string basepart(const string &fn)
{
	unsigned dot=fn.rfind('.');
	return fn.substr(0, dot);
}

string extpart(const string &fn)
{
	string::size_type dot=fn.rfind('.');
	if(dot==string::npos)
		return string();
	return fn.substr(dot);
}

Path fix_case(const Path &path)
{
	bool found=true;
	Path result;
	for(Path::Iterator i=path.begin(); i!=path.end(); ++i)
	{
		if(!found || *i=="/")
			result/=*i;
		else
		{
			list<string> files;
			if(result.size())
				files=list_files(result);
			else
				files=list_files(".");

			found=false;
			for(list<string>::iterator j=files.begin(); (j!=files.end() && !found); ++j)
				if(!strcasecmp(*j,*i))
				{
					result/=*j;
					found=true;
				}

			if(!found)
				result/=*i;
		}
	}

	return result;
}

Path readlink(const Path &link)
{
#ifdef WIN32
	(void)link;
	throw Exception("No symbolic links on win32");
#else
	char buf[4096];
	int len=::readlink(link.str().c_str(), buf, sizeof(buf));
	if(len==-1)
		throw SystemError("readlink failed", errno);
	return string(buf, len);
#endif
}

Path realpath(const Path &path)
{
#ifdef WIN32
	if(path.is_absolute())
		return path;
	else
		return getcwd()/path;
#else
	list<string> queue(path.begin(), path.end());
	if(!path.is_absolute())
	{
		Path cwd=getcwd();
		queue.insert(queue.begin(), cwd.begin(), cwd.end());
	}

	Path real;
	unsigned n_links=0;
	while(!queue.empty())
	{
		Path next=real/queue.front();
		queue.pop_front();

		struct stat st=lstat(next);
		if(S_ISLNK(st.st_mode))
		{
			if(++n_links>64)
				throw Exception("Ludicrous amount of symlinks detected in realpath, giving up");
			Path link=readlink(next);
			queue.insert(queue.begin(), link.begin(), link.end());
		}
		else
			real=next;
	}

	return real;
#endif
}

void unlink(const Path &path)
{
	if(::unlink(path.str().c_str())==-1)
		throw SystemError("unlink failed", errno);
}

Path relative(const Path &path, const Path &base)
{
	Path::Iterator i=path.begin();
	Path::Iterator j=base.begin();
	for(; (i!=path.end() && j!=base.end() && *i==*j); ++i, ++j) ;

	Path result;
	for(; j!=base.end(); ++j)
		result/="..";
	for(; i!=path.end(); ++i)
		result/=*i;

	return result;
}

} // namespace FS
} // namespace Msp
