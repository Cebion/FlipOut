#include <msp/time/utils.h>
#include "profilingscope.h"

using namespace std;

namespace Msp {
namespace Debug {

ProfilingScope::ProfilingScope(Profiler &p, const string &n):
	profiler(p),
	name(n),
	parent(profiler.enter(this)),
	entry_time(Time::now())
{ }

ProfilingScope::~ProfilingScope()
{
	time_spent = Time::now()-entry_time;
	if(parent)
		parent->child_time += get_time_spent();
	profiler.record(*this);
	profiler.enter(parent);
}

} // namespace Debug
} // namespace Msp
