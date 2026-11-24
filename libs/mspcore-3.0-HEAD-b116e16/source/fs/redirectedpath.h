#ifndef MSP_FS_REDIRECTEDPATH_H_
#define MSP_FS_REDIRECTEDPATH_H_

#include "path.h"

namespace Msp {
namespace FS {

/**
Provides a redirected path for the lifetime of the object.  When RedirectedPath
goes out of scope, it is renamed to the original path.  If destruction happens
due to an exception, it is unlinked instead.  The primary use for this is to
atomically overwrite a file with a new version.
*/
class RedirectedPath: public Path
{
private:
	Path original;

public:
	explicit RedirectedPath(const Path &, const char * = ".tmp");
	RedirectedPath(const Path &, const std::string &);
	RedirectedPath(const Path &, const Path &);
	~RedirectedPath();
};

} // namespace FS
} // namespace Msp

#endif
