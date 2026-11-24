/* $Id: profilingscope.h 35 2007-10-12 19:20:54Z tdb $

This file is part of libmspcore
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_DEBUG_PROFILINGSCOPE_H_
#define MSP_DEBUG_PROFILINGSCOPE_H_

#include "../time/timestamp.h"
#include "profiler.h"

namespace Msp {
namespace Debug {

/**
RAII timing class to accompany Profiler.  Timing starts when an object is
created and ends when it goes out of scope.  If there was another object in an
outer scope, it is notified of the time used in inner scopes.
*/
class ProfilingScope
{
private:
	Profiler &profiler;
	std::string name;
	ProfilingScope *parent;
	Time::TimeStamp start_t;
	Time::TimeDelta child_t;

	ProfilingScope(const ProfilingScope &);
	ProfilingScope &operator=(const ProfilingScope &);
public:
	ProfilingScope(Profiler &p, const std::string &n);
	~ProfilingScope();
};

} // namespace Debug
} // namespace Msp

#endif
