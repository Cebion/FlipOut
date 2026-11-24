#ifndef MSP_DEBUG_ERRORREPORTER_H_
#define MSP_DEBUG_ERRORREPORTER_H_

#include <stdexcept>
#include <msp/core/noncopyable.h>

namespace Msp {
namespace Debug {

class ErrorReporter: private NonCopyable
{
private:
	ErrorReporter *prev;

	static ErrorReporter *current;

protected:
	ErrorReporter();
public:
	virtual ~ErrorReporter();

	static const ErrorReporter *get_current() { return current; }

	virtual bool report_uncaught_exception(const std::exception &) const = 0;
};

} // namespace Debug
} // namespace Msp

#endif
