#include <msp/core/application.h>
#include <msp/core/environ.h>
#include "dir.h"

using namespace std;

namespace Msp {
namespace FS {

Path get_home_dir()
{
	string home = getenv("HOME");
	if(!home.empty())
		return home;
	return ".";
}

Path get_user_data_dir()
{
	const string &appname = Application::get_name();
	if(appname.empty())
		throw logic_error("no application name");

	return get_home_dir()/("."+appname);
}

} // namespace FS
} // namespace Msp
