#ifndef MSP_DEBUG_PROFILER_H_
#define MSP_DEBUG_PROFILER_H_

#include <map>
#include <string>
#include <vector>
#include <msp/core/noncopyable.h>
#include <msp/time/timedelta.h>
#include <msp/time/timestamp.h>

namespace Msp {
namespace Debug {

class ProfilingScope;

/**
A class for collecting timing data from a program.  It's not as efficient as
external profilers, but allows profiling of custom scopes and retrieving the
profiling data at run time.  An example usage could be showing realtime
performance statistics of a game or a simulation.

Data can be recorded by creating a ProfilingScope object in the scope to be
profiled.

Note: This is not thread-safe.  To profile multiple threads, create a separate
Profiler for each thread.
*/
class Profiler: private NonCopyable
{
public:
	struct CallInfo
	{
		Msp::Time::TimeStamp entry_time;
		Msp::Time::TimeDelta duration;
	};

	struct ScopeInfo
	{
		Time::TimeStamp first_call;
		unsigned calls;
		Time::TimeDelta total_time;
		Time::TimeDelta self_time;
		Time::TimeDelta avg_time;
		float calls_per_sec;
		std::vector<CallInfo> history;
		unsigned hist_pos;
		bool hist_full;
		std::map<std::string, unsigned> called_from;

		ScopeInfo();
	};

private:
	unsigned period;
	std::map<std::string, ScopeInfo> scopes;
	ProfilingScope *inner;

public:
	Profiler();

	/** Sets the averaging period for timing data, measured in calls.  Previous
	average timings are cleared. */
	void set_period(unsigned p);

	/** Adds a scope without recording any calls to it.  Useful if you might
	need to access a scope before it has finished for the first time. */
	void add_scope(const std::string &name);

	/** Changes the recorded innermost scope pointer and returns the old one.
	This is used by ProfilingScope to track child time and should not be called
	manually. */
	ProfilingScope *enter(ProfilingScope *ps);

	/** Records the data from a ProfilingScope.  It is not useful to call this
	manually. */
	void record(const ProfilingScope &);

	/** Returns informations about a scope. */
	const ScopeInfo &get_scope(const std::string &) const;
};

} // namespace Debug
} // namespace Msp

#endif
