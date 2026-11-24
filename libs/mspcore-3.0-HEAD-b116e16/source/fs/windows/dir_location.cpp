#include <shlobj.h>
#include <msp/core/application.h>
#include "dir.h"

using namespace std;

namespace Msp {
namespace FS {

Path get_home_dir()
{
	char home[MAX_PATH];
	if(SHGetFolderPath(0, CSIDL_PERSONAL, 0, 0, home)==S_OK)
		return home;
	return ".";
}

Path get_user_data_dir()
{
	const string &appname = Application::get_name();
	if(appname.empty())
		throw logic_error("no application name");

	char datadir[MAX_PATH];
	if(SHGetFolderPath(0, CSIDL_LOCAL_APPDATA, 0, 0, datadir)==S_OK)
		return Path(datadir)/appname;
	return ".";
}

} // namespace FS
} // namespace Msp
