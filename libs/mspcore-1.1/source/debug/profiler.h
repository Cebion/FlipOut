/* $Id: profiler.h 35 2007-10-12 19:20:54Z tdb $

This file is part of libmspcore
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_DEBUG_PROFILER_H_
#define MSP_DEBUG_PROFILER_H_

#include <map>
#include <string>
#include <vector>
#include "../time/timedelta.h"

namespace Msp {
namespace Debug {

class ProfilingScope;

/**
A class for collecting timing data from a program.  It's not as efficient as
external profilers, but allows profiling of custom scopes and retrieving the
profiling data at run time.  An example usage could be showing realtime
performance statistics of a game or a simulation.

See also class ProfilingScope.

Note: This is not thread-safe.  To profile multiple threads, create a separate
Profiler for each thread.
*/
class Profiler
{
public:
	struct ScopeInfo
	{
		unsigned calls;
		Time::TimeDelta total_time;
		Time::TimeDelta self_time;
		Time::TimeDelta avg_time;
		std::vector<Time::TimeDelta> history;
		unsigned hist_pos;
		std::map<std::string, unsigned> called_from;

		ScopeInfo();
	};

private:
	unsigned period;
	std::map<std::string, ScopeInfo> scopes;
	ProfilingScope *inner;

public:
	Profiler();

	/**
	Sets the averaging period for timing data, measured in calls.  Previous
	average timings are cleared.
	*/
	void set_period(unsigned p);

	/**
	Adds a scope without recording any calls to it.  Useful if you might need to
	access a scope before it has finished for the first time.
	*/
	void add_scope(const std::string &name);

	/**
	Changes the recorded innermost scope pointer and returns the old one.  This
	is used by ProfilingScope to track child time and should not be called
	manually.
	*/
	ProfilingScope *enter(ProfilingScope *ps);

	/**
	Records a call to a scope.  You'll probably want to use a ProfilingScope
	instead of calling this manually.

	@param  sn  Scope name
	@param  pn  Parent scope name
	@param  t   Time spent in the scope
	@param  ct  Time spent in child scopes
	*/
	void record(const std::string &sn, const std::string &pn, const Time::TimeDelta &t, const Time::TimeDelta &ct);

	/**
	Returns informations about a scope.
	*/
	const ScopeInfo &scope(const std::string &) const;
};

} // namespace Debug
} // namespace Msp

#endif
