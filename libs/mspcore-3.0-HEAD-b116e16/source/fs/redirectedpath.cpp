#include <exception>
#include "redirectedpath.h"
#include "stat.h"
#include "utils.h"

using namespace std;

namespace Msp {
namespace FS {

RedirectedPath::RedirectedPath(const Path &p, const char *ext):
	Path(p.str()+ext),
	original(p)
{ }

RedirectedPath::RedirectedPath(const Path &p, const string &ext):
	Path(p.str()+ext),
	original(p)
{ }

RedirectedPath::RedirectedPath(const Path &p, const Path &r):
	Path(r),
	original(p)
{ }

RedirectedPath::~RedirectedPath()
{
	if(exists(*this))
	{
		if(uncaught_exception())
			unlink(*this);
		else
			rename(*this, original);
	}
}

} // namespace FS
} // namespace Msp
