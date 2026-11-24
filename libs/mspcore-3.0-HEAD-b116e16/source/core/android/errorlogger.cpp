#include <typeinfo>
#include <android/log.h>
#include <msp/debug/demangle.h>
#include "errorlogger.h"

using namespace std;

namespace Msp {
namespace Android {

bool ErrorLogger::report_uncaught_exception(const exception &e) const
{
	__android_log_write(ANDROID_LOG_ERROR, "Msp", "An uncaught exception occurred.");
	__android_log_print(ANDROID_LOG_ERROR, "Msp", "  type:   %s", Debug::demangle(typeid(e).name()).c_str());
	__android_log_print(ANDROID_LOG_ERROR, "Msp", "  what(): %s", e.what());
	return true;
}

} // namespace Android
} // namespace Msp
