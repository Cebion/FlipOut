/* $Id: profilingscope.cpp 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "../time/utils.h"
#include "profilingscope.h"

using namespace std;

namespace Msp {
namespace Debug {

ProfilingScope::ProfilingScope(Profiler &p, const string &n):
	profiler(p),
	name(n),
	parent(profiler.enter(this)),
	start_t(Time::now())
{ }

ProfilingScope::~ProfilingScope()
{
	const Time::TimeDelta dt=Time::now()-start_t;
	if(parent)
	{
		parent->child_t+=dt;
		profiler.record(name, parent->name, dt, child_t);
	}
	else
		profiler.record(name, string(), dt, child_t);
	profiler.enter(parent);
}

} // namespace Debug
} // namespace Msp
