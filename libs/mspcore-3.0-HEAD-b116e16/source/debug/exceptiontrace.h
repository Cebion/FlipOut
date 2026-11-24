#ifndef MSP_DEBUG_EXCEPTIONTRACE_H_
#define MSP_DEBUG_EXCEPTIONTRACE_H_

namespace Msp {
namespace Debug {

class Backtrace;

void enable_exception_trace(bool);
const Backtrace &get_exception_trace();

} // namespace Debug
} // namespace Msp

#endif
