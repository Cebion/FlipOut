#include <msp/debug/demangle.h>
#include <msp/strings/format.h>
#include <msp/strings/utils.h>
#include "maputils.h"

using namespace std;

namespace Msp {

string key_error::make_what(const type_info &type, const string &value)
{
	if(&type==&typeid(string))
		return format("\"%s\"", c_escape(value));
	else
		return format("%s(%s)", Debug::demangle(type.name()), value);
}

} // namespace Msp
