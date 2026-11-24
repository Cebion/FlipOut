#include <limits>
#include "systemerror.h"

using namespace std;

namespace Msp {

system_error::system_error(const string &w, int c):
	runtime_error(w+": "+get_message(c)),
	code_(c)
{ }

system_error::system_error(const string &w, const string &e):
	runtime_error(w+": "+e),
	code_(numeric_limits<int>::min())
{ }

} // namespace Msp
