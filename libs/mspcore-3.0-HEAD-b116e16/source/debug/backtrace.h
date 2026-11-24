#ifndef MSP_DEBUG_BACKTRACE_H_
#define MSP_DEBUG_BACKTRACE_H_

#include <ostream>
#include <string>
#include <vector>

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
	std::vector<StackFrame> frames;

public:
	const std::vector<StackFrame> &get_frames() const { return frames; }

	static Backtrace create();
};

std::ostream &operator<<(std::ostream &, const Backtrace &);
std::ostream &operator<<(std::ostream &, const Backtrace::StackFrame &);

} // namespace Debug
} // namespace Msp

#endif
