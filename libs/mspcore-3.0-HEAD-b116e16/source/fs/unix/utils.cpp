#include <cstdio>
#include <deque>
#include <unistd.h>
#include <msp/core/systemerror.h>
#include "dir.h"
#include "stat.h"
#include "utils.h"

using namespace std;

namespace Msp {
namespace FS {

Path readlink(const Path &link)
{
	char buf[4096];
	int len = ::readlink(link.c_str(), buf, sizeof(buf));
	if(len==-1)
		throw system_error("readlink");
	return string(buf, len);
}

Path realpath(const Path &path)
{
	deque<string> queue(path.begin(), path.end());
	if(!path.is_absolute())
	{
		Path cwd = getcwd();
		queue.insert(queue.begin(), cwd.begin(), cwd.end());
	}

	Path real;
	unsigned n_links = 0;
	while(!queue.empty())
	{
		Path next = real/queue.front();
		queue.pop_front();

		if(is_link(next))
		{
			if(++n_links>64)
				throw runtime_error("too many symbolic links");
			Path link = readlink(next);
			queue.insert(queue.begin(), link.begin(), link.end());
		}
		else
			real = next;
	}

	return real;
}

void rename(const Path &from, const Path &to)
{
	if(::rename(from.c_str(), to.c_str())==-1)
		throw system_error("rename");
}

void unlink(const Path &path)
{
	if(::unlink(path.c_str())==-1)
		throw system_error("unlink");
}

} // namespace FS
} // namespace Msp
