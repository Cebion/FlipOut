#include <stdexcept>
#include <msp/strings/utils.h>
#include "path.h"
#include "utils.h"

using namespace std;

namespace {

#ifdef _WIN32
inline bool is_windows_drive(const string &p)
{ return (p.size()==2 && ((p[0]>='A' && p[0]<='Z') || (p[0]>='a' && p[0]<='z')) && p[1]==':'); }
#endif

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

void Path::init(const string &p)
{
	if(p.empty())
		return;
	string::size_type start = 0;
	while(start<p.size())
	{
		string::size_type slash = p.find_first_of("/\\", start);
		if(slash>start || start==0)
			add_component(p.substr(start, max<string::size_type>(slash-start, 1U)));
		if(slash==string::npos)
			break;
		start = slash+1;
	}
}

unsigned Path::size() const
{
	if(path.empty())
		return 0;
	if(path.size()==1 && path[0]==DIRSEP)
		return 1;

	return separators.size()+1;
}

bool Path::is_absolute() const
{
#ifdef _WIN32
	if(!empty() && is_windows_drive((*this)[0]))
		return true;
#endif
	return path[0]==DIRSEP;
}

Path Path::subpath(unsigned start, unsigned count) const
{
	Path result;
	auto i = begin();
	for(unsigned j=0; (j<start && i!=end()); ++j)
		++i;
	for(unsigned j=0; (j<count && i!=end()); ++j)
	{
		result /= *i;
		++i;
	}
	return result;
}

Path Path::operator/(const Path &p) const
{
	Path a = *this;
	a /= p;
	return a;
}

Path &Path::operator/=(const Path &p)
{
	if(p.is_absolute())
		*this = p;
	else
	{
		for(const string &c: p)
			add_component(c);
	}
	return *this;
}

void Path::add_component(const string &comp)
{
	if(comp.size()==1 && (comp[0]=='/' || comp[0]=='\\'))
	{
		// Replace the path with the root directory
#ifdef _WIN32
		string::size_type slash = (separators.empty() ? string::npos : separators.front());
		if(is_windows_drive(path.substr(0, slash)))
		{
			path = path.substr(0, 2);
			separators.clear();
		}
		else
#endif
		{
			path.assign(1, DIRSEP);
			separators.clear();
			separators.push_back(0);
		}
	}
#ifdef _WIN32
	else if(is_windows_drive(comp))
	{
		path = comp;
		separators.clear();
	}
#endif
	else if(comp=="..")
	{
		if(path.empty() || path==".")
			path = comp;
		// .. in root directory is a no-op
		else if(path.size()==1 && path[0]==DIRSEP)
			;
#ifdef _WIN32
		else if(is_windows_drive(path))
			;
#endif
		else
		{
			string::size_type start = (separators.empty() ? 0 : separators.back()+1);
			if(!path.compare(start, string::npos, ".."))
			{
				// If the last component already is a .., add another
				separators.push_back(path.size());
				path += DIRSEP;
				path += comp;
			}
			else if(separators.empty())
				path = ".";
			else
			{
				// Otherwise, erase the last component
				path.erase(separators.back(), string::npos);
				separators.pop_back();
			}
		}
	}
	else if(comp!="." || path.empty())
	{
		if(comp!="." && path.empty())
			path = ".";
		if(path.size()>1 || (path.size()==1 && path[0]!=DIRSEP))
		{
			separators.push_back(path.size());
			path += DIRSEP;
		}
		path += comp;
	}
}

string Path::operator[](int n) const
{
	if(n>=0)
	{
		for(auto i=begin(); i!=end(); ++i, --n)
			if(!n)
				return *i;
	}
	else
	{
		for(auto i=end(); i!=begin();)
		{
			--i;
			if(!++n)
				return *i;
		}
	}

	throw invalid_argument("Path::operator[]");
}

bool Path::operator==(const Path &other) const
{
#ifdef _WIN32
	return strcasecmp(path, other.path)==0;
#else
	return path==other.path;
#endif
}

bool Path::operator<(const Path &other) const
{
#ifdef _WIN32
	return strcasecmp(path, other.path)<0;
#else
	return path<other.path;
#endif
}

bool Path::operator>(const Path &other) const
{
#ifdef _WIN32
	return strcasecmp(path, other.path)>0;
#else
	return path>other.path;
#endif
}


Path::Iterator::Iterator(const Path &p, bool e):
	path(&p),
	iter(e ? path->separators.end() : path->separators.begin()),
	end(e || path->path.empty())
{
	update();
}

Path::Iterator &Path::Iterator::operator++()
{
	if(iter==path->separators.end())
		end = true;
	else
	{
		++iter;
		if(path->path.size()==1 && path->separators.size()==1)
			end = true;
	}
	update();
	return *this;
}

Path::Iterator &Path::Iterator::operator--()
{
	if(end)
	{
		end = false;
		if(path->path.size()==1 && path->separators.size()==1)
			--iter;
	}
	else if(iter!=path->separators.begin())
		--iter;
	update();
	return *this;
}

void Path::Iterator::update()
{
	if(end)
	{
		current.clear();
		return;
	}

	string::size_type start = 0;
	if(iter!=path->separators.begin())
		start = *prev(iter)+1;

	string::size_type slash = string::npos;
	if(iter!=path->separators.end())
		slash = *iter;

	if(slash==0)
		current = path->path.substr(start, 1);
	else
		current = path->path.substr(start, slash-start);
}

} // namespace FS
} // namespace Msp
