#include <msp/core/application.h>
#include <msp/core/environ.h>
#include "dir.h"

using namespace std;

extern "C" unsigned get_home_dir(char *, unsigned);
extern "C" unsigned get_application_support_dir(char *, unsigned);

namespace Msp {
namespace FS {

Path get_home_dir()
{
	char buf[1024];
	unsigned len = ::get_home_dir(buf, sizeof(buf));
	if(len)
		return string(buf, len);

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

	char buf[1024];
	unsigned len = get_application_support_dir(buf, sizeof(buf));
	if(len)
		return Path(string(buf, len))/appname;
	return get_home_dir()/"Library"/"Application Support"/appname;
}

} // namespace FS
} // namespace Msp
