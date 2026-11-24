#include <windows.h>
#include <msp/core/systemerror.h>
#include "dir.h"
#include "utils.h"

using namespace std;

namespace Msp {
namespace FS {

Path readlink(const Path &link)
{
	(void)link;
	throw logic_error("no symbolic links on win32");
}

Path realpath(const Path &path)
{
	if(path.is_absolute())
		return path;
	else
		return getcwd()/path;
}

void rename(const Path &from, const Path &to)
{
	if(!MoveFileEx(from.c_str(), to.c_str(), MOVEFILE_REPLACE_EXISTING))
		throw system_error("MoveFileEx");
}

void unlink(const Path &path)
{
	if(!DeleteFile(path.c_str()))
		throw system_error("DeleteFile");
}

} // namespace FS
} // namespace Msp
