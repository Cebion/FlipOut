#include <string>
#if !defined(_WIN32) && defined(__GLIBC__)
#include <dlfcn.h>
#endif
#include "backtrace.h"
#include "exceptiontrace.h"

using namespace std;

namespace {

#if defined(WITH_EXCEPTION_TRACE) && !defined(_WIN32) && defined(__GLIBC__)
typedef void CxaThrowType(void *, type_info *, void (*)(void *));
CxaThrowType *orig_cxa_throw;

struct HookThrow
{
	HookThrow();
};

HookThrow::HookThrow()
{
	void *handle = dlopen(0, RTLD_LAZY);
	orig_cxa_throw = reinterpret_cast<CxaThrowType *>(dlvsym(handle, "__cxa_throw", "CXXABI_1.3"));
	dlclose(handle);
}

HookThrow hook;
#endif

bool trace_enabled = false;

Msp::Debug::Backtrace &get_thread_backtrace()
{
	static thread_local Msp::Debug::Backtrace backtrace;
	return backtrace;
}

}

namespace Msp {
namespace Debug {

void enable_exception_trace(bool e)
{
	trace_enabled = e;
}

const Backtrace &get_exception_trace()
{
	return get_thread_backtrace();
}

} // namespace Debug
} // namespace Msp

#if defined(WITH_EXCEPTION_TRACE) && !defined(_WIN32) && defined(__GLIBC__)
extern "C" void __cxa_throw(void *exc, type_info *type, void (*dest) (void *))
{
	if(trace_enabled)
		get_thread_backtrace() = Msp::Debug::Backtrace::create();
	orig_cxa_throw(exc, type, dest);
	terminate();
}
#endif
