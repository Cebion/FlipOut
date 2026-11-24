#ifndef MSP_DEBUG_PROFILINGSCOPE_H_
#define MSP_DEBUG_PROFILINGSCOPE_H_

#include <msp/core/noncopyable.h>
#include <msp/time/timestamp.h>
#include "profiler.h"

namespace Msp {
namespace Debug {

/**
RAII timing class to accompany Profiler.  Timing starts when an object is
created and ends when it goes out of scope.  If there was another object in an
outer scope, it is notified of the time used in inner scopes.
*/
class ProfilingScope: private NonCopyable
{
private:
	Profiler &profiler;
	std::string name;
	ProfilingScope *parent;
	Time::TimeStamp entry_time;
	Time::TimeDelta time_spent;
	Time::TimeDelta child_time;

	ProfilingScope(const ProfilingScope &);
	ProfilingScope &operator=(const ProfilingScope &);
public:
	ProfilingScope(Profiler &p, const std::string &n);
	~ProfilingScope();

	const std::string &get_name() const { return name; }
	const ProfilingScope *get_parent() const { return parent; }
	const Time::TimeStamp &get_entry_time() const { return entry_time; }
	const Time::TimeDelta &get_time_spent() const { return time_spent; }
	const Time::TimeDelta &get_child_time() const { return child_time; }
};

} // namespace Debug
} // namespace Msp

#endif
