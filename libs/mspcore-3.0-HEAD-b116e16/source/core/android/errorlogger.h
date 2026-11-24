#ifndef MSP_CORE_ERRORLOGGER_ANDROID_H_
#define MSP_CORE_ERRORLOGGER_ANDROID_H_

#include <msp/debug/errorreporter.h>

namespace Msp {
namespace Android {

class ErrorLogger: public Debug::ErrorReporter
{
public:
	virtual bool report_uncaught_exception(const std::exception &) const;
};

} // namespace Android
} // namespace Msp

#endif
