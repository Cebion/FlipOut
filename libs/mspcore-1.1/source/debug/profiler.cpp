/* $Id: profiler.cpp 42 2007-12-28 20:47:02Z tdb $

This file is part of libmspcore
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "../core/except.h"
#include "../time/units.h"
#include "profiler.h"

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

	period=p;
	for(map<string, ScopeInfo>::iterator i=scopes.begin(); i!=scopes.end(); ++i)
	{
		ScopeInfo &si=i->second;
		if(p==0)
			si.history.clear();
		else
			si.history.assign(period, Time::zero);
		si.hist_pos=0;
	}
}

void Profiler::add_scope(const std::string &name)
{
	if(!scopes.count(name))
	{
		map<string, ScopeInfo>::iterator i=scopes.insert(map<string, ScopeInfo>::value_type(name, ScopeInfo())).first;
		i->second.history.resize(period);
	}
}

ProfilingScope *Profiler::enter(ProfilingScope *ps)
{
	ProfilingScope *old=inner;
	inner=ps;
	return old;
}

void Profiler::record(const string &scope_name, const string &parent, const Time::TimeDelta &time, const Time::TimeDelta &child_t)
{
	map<string, ScopeInfo>::iterator i=scopes.find(scope_name);
	if(i==scopes.end())
	{
		i=scopes.insert(map<string, ScopeInfo>::value_type(scope_name, ScopeInfo())).first;
		i->second.history.resize(period);
	}

	ScopeInfo &si=i->second;
	++si.calls;
	++si.called_from[parent];
	si.total_time+=time;
	si.self_time+=time-child_t;
	if(period)
	{
		si.avg_time+=time/period-si.history[si.hist_pos]/period;
		si.history[si.hist_pos++]=time;
		if(si.hist_pos>=period)
			si.hist_pos-=period;
	}
	else
		si.avg_time=si.total_time/si.calls;
}

const Profiler::ScopeInfo &Profiler::scope(const string &sn) const
{
	map<string, ScopeInfo>::const_iterator i=scopes.find(sn);
	if(i==scopes.end())
		throw KeyError("Unknown scope");

	return i->second;
}


Profiler::ScopeInfo::ScopeInfo():
	calls(0),
	hist_pos(0)
{ }

} // namespace Debug
} // namespace Msp
