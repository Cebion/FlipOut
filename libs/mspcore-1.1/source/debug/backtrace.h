/* $Id: backtrace.h 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_DEBUG_BACKTRACE_H_
#define MSP_DEBUG_BACKTRACE_H_

#include <list>
#include <ostream>
#include <string>

namespace Msp {
namespace Debug {

class Backtrace
{
public:
	struct StackFrame
	{
		void *address;
		std::string file;
		std::string symbol;
	};

private:
	std::list<StackFrame> frames;

public:
	const std::list<StackFrame> &get_frames() const { return frames; }

	static Backtrace create();
};

std::ostream &operator<<(std::ostream &, const Backtrace &);
std::ostream &operator<<(std::ostream &, const Backtrace::StackFrame &);

} // namespace Debug
} // namespace Msp

#endif
