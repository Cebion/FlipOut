/* $Id$

This file is part of libmspfs
Copyright © 2006-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/core/except.h>
#include <msp/strings/utils.h>
#include "path.h"
#include "utils.h"

using namespace std;

namespace {

inline bool is_windows_drive(const std::string &p)
{ return (p.size()==2 && ((p[0]>='A' && p[0]<='Z') || (p[0]>='a' && p[0]<='z')) && p[1]==':'); }

}

namespace Msp {
namespace FS {

Path::Path()
{ }

Path::Path(const string &p)
{
	init(p);
}

Path::Path(const char *p)
{
	init(p);
}

unsigned Path::size() const
{
	if(path.empty())
		return 0;
	if(path.size()==1 && path[0]==DIRSEP)
		return 1;

	unsigned count=1;
	for(string::const_iterator i=path.begin(); i!=path.end(); ++i)
		if(*i==DIRSEP) ++count;
	return count;
}

bool Path::is_absolute() const
{
#ifdef WIN32
	if(is_windows_drive((*this)[0]))
		return true;
#endif
	if(path[0]==DIRSEP)
		return true;
	return false;
}

Path Path::subpath(unsigned start, unsigned count) const
{
	Path result;
	Iterator i=begin();
	for(unsigned j=0; (j<start && i!=end()); ++j)
		++i;
	for(unsigned j=0; (j<count && i!=end()); ++j)
	{
		result/=*i;
		++i;
	}
	return result;
}

Path Path::operator/(const Path &p) const
{
	Path a=*this;
	a/=p;
	return a;
}

/**
Attaches another path to the end of this one.  An absolute path replaces the
existing data.  ".." elements annihilate the last component and "." elements
are ignored.
*/
Path &Path::operator/=(const Path &p)
{
	if(p.is_absolute())
		path=p.path;
	else
	{
		for(Iterator i=p.begin(); i!=p.end(); ++i)
			add_component(*i);
	}
	return *this;
}

string Path::operator[](int n) const
{
	if(n>=0)
	{
		for(Iterator i=begin(); i!=end(); ++i, --n)
			if(!n)
				return *i;
	}
	else
	{
		for(Iterator i=end(); i!=begin();)
		{
			--i;
			if(!++n)
				return *i;
		}
	}

	throw InvalidParameterValue("Path component index out of range");
}

bool Path::operator==(const Path &p) const
{
#ifdef WIN32
	return !strcasecmp(path, p.path);
#else
	return path==p.path;
#endif
}

Path::Iterator Path::begin() const
{
	return Iterator(*this);
}

Path::Iterator Path::end() const
{
	Iterator i(*this);
	i.start=i.end=std::string::npos;
	return i;
}

void Path::init(const string &p)
{
	string::size_type start=0;
	if(p[0]=='/' || p[0]=='\\')
		add_component(string(1, DIRSEP));
	while(1)
	{
		string::size_type slash=p.find_first_of("/\\", start);
		if(slash>start)
			add_component(p.substr(start, slash-start));
		if(slash==string::npos)
			break;
		start=slash+1;
	}
}

/**
Adds a single component to the path, emulating the cd command.  Fails horribly
if comp contains a separator character.
*/
void Path::add_component(const string &comp)
{
	if(comp.empty())
		;
	else if(comp.size()==1 && comp[0]==DIRSEP)
	{
		// Replace the path with the root directory
#ifdef WIN32
		unsigned slash=path.find(DIRSEP);
		if(is_windows_drive(path.substr(0, slash)))
			path=path.substr(0, 2);
		else
#endif
		path=comp;
	}
#ifdef WIN32
	else if(is_windows_drive(comp))
		path=comp;
#endif
	else if(comp=="..")
	{
		if(path.empty() || path==".")
			path=comp;
		// .. in root directory is a no-op
		else if(path.size()==1 && path[0]==DIRSEP)
			;
#ifdef WIN32
		else if(is_windows_drive(path))
			;
#endif
		else
		{
			string::size_type slash=path.rfind(DIRSEP);
			string::size_type start=(slash==string::npos ? 0 : slash+1);
			if(!path.compare(start, string::npos, ".."))
			{
				// If the last component already is a .., add another
				path+=DIRSEP;
				path+=comp;
			}
			else if(slash==string::npos)
				path=".";
			else
			{
				if(slash==0)
					slash=1;
				// Otherwise, erase the last component
				path.erase(slash, string::npos);
			}
		}
	}
	else if(comp!="." || path.empty())
	{
		if(comp!="." && path.empty())
			path=".";
		if(path.size()>1 || (path.size()==1 && path[0]!=DIRSEP))
			path+=DIRSEP;
		path+=comp;
	}
}


Path::Iterator::Iterator(const Path &p):
	path(p),
	start(0)
{
	if(path.path.empty())
		start=end=string::npos;
	else if(path.path[0]==DIRSEP)
		end=1;
#ifdef WIN32
	else if(path.path.size()>2 && path.path[2]==DIRSEP && is_windows_drive(path.path.substr(0, 2)))
		end=2;
#endif
	else
		end=path.path.find(DIRSEP);
}

Path::Iterator &Path::Iterator::operator++()
{
	start=end;
	if(start>=path.path.size())
		return *this;
	if(path.path[start]==DIRSEP)
		++start;
	end=path.path.find(DIRSEP, start);
	return *this;
}

Path::Iterator &Path::Iterator::operator--()
{
	if(start==0)
		return *this;

	end=start;
	if(end>1 && end<path.path.size() && path.path[end]!=DIRSEP)
		--end;

	start=path.path.rfind(DIRSEP, end-1);
	if(start==string::npos)
		start=0;
	else if(start<end-1)
		++start;

	return *this;
}

string Path::Iterator::operator*() const
{
	if(start>=path.path.size())
		return string();
	if(start==end)
		return string();
	return path.path.substr(start, end-start);
}

} // namespace FS
} // namespace Msp
