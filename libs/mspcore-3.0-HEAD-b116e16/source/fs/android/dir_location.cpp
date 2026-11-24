#include <msp/core/application.h>
#include <msp/core/mainthread.h>
#include "dir.h"

using namespace std;

namespace Msp {
namespace FS {

Path get_home_dir()
{
	return "/sdcard";
}

Path get_user_data_dir()
{
	Android::MainThread *thread = reinterpret_cast<Android::MainThread *>(Application::get_data());
	return thread->get_internal_data_path();
}

} // namespace FS
} // namespace Msp
