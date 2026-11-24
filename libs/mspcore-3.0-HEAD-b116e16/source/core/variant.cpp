#include <msp/debug/demangle.h>
#include <msp/strings/format.h>
#include "variant.h"

using namespace std;

namespace Msp {

type_mismatch::type_mismatch(const type_info &e, const type_info &a):
	runtime_error(format("expected: %s\nactual: %s", Debug::demangle(e.name()), Debug::demangle(a.name())))
{ }

}
