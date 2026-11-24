#include "errorreporter.h"

namespace Msp {
namespace Debug {

ErrorReporter *ErrorReporter::current = 0;

ErrorReporter::ErrorReporter():
	prev(current)
{
	current = this;
}

ErrorReporter::~ErrorReporter()
{
	current = prev;
}

} // namespace Debug
} // namespace Msp
