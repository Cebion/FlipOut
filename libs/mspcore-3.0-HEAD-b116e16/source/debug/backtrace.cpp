// Must include something to test for glibc
#include <cstdlib>
#if !defined(_WIN32) && defined(__GLIBC__)
#include <dlfcn.h>
#include <execinfo.h>
#endif
#include "backtrace.h"
#include "demangle.h"

using namespace std;

namespace Msp {
namespace Debug {

Backtrace Backtrace::create()
{
#if !defined(_WIN32) && defined(__GLIBC__)
	void *addresses[50];
	int count = ::backtrace(addresses, 50);

	Backtrace bt;
	Dl_info dli;
	for(int i=0; i<count; ++i)
	{
		StackFrame frame;
		frame.address = addresses[i];
		if(dladdr(addresses[i], &dli))
		{
			frame.file = dli.dli_fname;
			if(dli.dli_sname)
				frame.symbol = demangle(dli.dli_sname);
		}
		else
			frame.file = "<unknown>";
		bt.frames.push_back(frame);
	}

	return bt;
#else
	return Backtrace();
#endif
}

ostream &operator<<(ostream &out, const Backtrace &bt)
{
	for(const Backtrace::StackFrame &f: bt.get_frames())
		out << f << '\n';

	return out;
}

ostream &operator<<(ostream &out, const Backtrace::StackFrame &sf)
{
	out << sf.address;
	if(!sf.symbol.empty())
		out << " in " << sf.symbol;
	out << " from " << sf.file;

	return out;
}

} // namespace Debug
} // namespace Msp
