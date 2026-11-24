#include <msp/core/maputils.h>
#include "profiler.h"
#include "profilingscope.h"

using namespace std;

namespace Msp {
namespace Debug {

Profiler::Profiler():
	period(0),
	inner(0)
{ }

void Profiler::set_period(unsigned p)
{
	if(p==period)
		return;

	period = p;
	for(auto &kvp: scopes)
	{
		ScopeInfo &si = kvp.second;
		if(p==0)
			si.history.clear();
		else
			si.history.assign(period, CallInfo());
		si.hist_pos = 0;
		si.hist_full = false;
	}
}

void Profiler::add_scope(const string &name)
{
	if(!scopes.count(name))
	{
		auto i = scopes.insert(make_pair(name, ScopeInfo())).first;
		i->second.history.resize(period);
	}
}

ProfilingScope *Profiler::enter(ProfilingScope *ps)
{
	ProfilingScope *old = inner;
	inner = ps;
	return old;
}

void Profiler::record(const ProfilingScope &scope)
{
	auto i = scopes.find(scope.get_name());
	if(i==scopes.end())
	{
		i = scopes.insert(make_pair(scope.get_name(), ScopeInfo())).first;
		i->second.first_call = scope.get_entry_time();
		i->second.history.resize(period);
	}

	ScopeInfo &si = i->second;
	++si.calls;
	if(scope.get_parent())
		++si.called_from[scope.get_parent()->get_name()];
	si.total_time += scope.get_time_spent();
	si.self_time += scope.get_time_spent()-scope.get_child_time();
	if(period)
	{
		si.avg_time += scope.get_time_spent()/period-si.history[si.hist_pos].duration/period;

		CallInfo &ci = si.history[si.hist_pos];
		Time::TimeStamp old_entry = ci.entry_time;
		ci.entry_time = scope.get_entry_time();
		ci.duration = scope.get_time_spent();

		++si.hist_pos;
		if(si.hist_pos>=period)
			si.hist_pos -= period;

		if(si.hist_full)
			si.calls_per_sec = period*Time::sec/(scope.get_entry_time()-old_entry);
		else if(si.hist_pos>1)
			si.calls_per_sec = (si.hist_pos-1)*Time::sec/(scope.get_entry_time()-si.history.front().entry_time);

		if(si.hist_pos==0)
			si.hist_full = true;
	}
	else
	{
		si.avg_time = si.total_time/si.calls;
		if(si.calls>1)
			si.calls_per_sec = (si.calls-1)*Time::sec/(scope.get_entry_time()-si.first_call);
	}
}

const Profiler::ScopeInfo &Profiler::get_scope(const string &sn) const
{
	return get_item(scopes, sn);
}


Profiler::ScopeInfo::ScopeInfo():
	calls(0),
	calls_per_sec(0),
	hist_pos(0),
	hist_full(false)
{ }

} // namespace Debug
} // namespace Msp
