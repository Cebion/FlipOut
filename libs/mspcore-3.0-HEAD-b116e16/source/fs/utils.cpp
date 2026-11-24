#include <msp/strings/utils.h>
#include "dir.h"
#include "utils.h"

using namespace std;

namespace Msp {
namespace FS {

string basename(const Path &p)
{
	return p.empty() ? string() : p[-1];
}

Path dirname(const Path &p)
{
	if(p.empty())
		return p;
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
	unsigned dot = fn.rfind('.');
	return fn.substr(0, dot);
}

string extpart(const string &fn)
{
	string::size_type dot = fn.rfind('.');
	if(dot==string::npos)
		return string();
	return fn.substr(dot);
}

Path fix_case(const Path &path)
{
	bool found = true;
	Path result;
	for(const string &c: path)
	{
		if(!found || (result.empty() && (c=="/" || c==".")))
			result /= c;
		else
		{
			vector<string> files;
			if(result.size())
				files = list_files(result);
			else
				files = list_files(".");

			found = false;
			for(const string &f: files)
				if(!strcasecmp(f, c))
				{
					result /= f;
					found = true;
				}

			if(!found)
				result /= c;
		}
	}

	return result;
}

Path relative(const Path &path, const Path &base)
{
	if(path.is_absolute()!=base.is_absolute())
		throw invalid_argument("FS::relative");

	auto i = path.begin();
	auto j = base.begin();
	for(; (i!=path.end() && j!=base.end() && *i==*j); ++i, ++j) ;

	Path result;
	for(; j!=base.end(); ++j)
		result /= "..";
	for(; i!=path.end(); ++i)
		result /= *i;

	return result;
}

Path common_ancestor(const Path &path1, const Path &path2)
{
	auto i = path1.begin();
	auto j = path2.begin();
	Path result;
	for(; (i!=path1.end() && j!=path2.end() && *i==*j); ++i, ++j)
		result /= *i;
	return result;
}

int descendant_depth(const Path &path, const Path &parent)
{
	if(path.is_absolute()!=parent.is_absolute())
		throw invalid_argument("FS::descendant_depth");

	auto i = path.begin();
	auto j = parent.begin();
	for(; (i!=path.end() && j!=parent.end() && *i==*j); ++i, ++j) ;

	if(j!=parent.end())
		return -1;
	
	int result = 0;
	for(; i!=path.end(); ++i)
		++result;

	return result;
}

} // namespace FS
} // namespace Msp
