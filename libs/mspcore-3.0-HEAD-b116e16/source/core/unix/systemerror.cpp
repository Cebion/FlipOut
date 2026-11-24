#include <cerrno>
#include <cstring>
#include "systemerror.h"

using namespace std;

namespace Msp {

string system_error::get_message(int c)
{
	if(c==-1)
		c = errno;

	return strerror(c);
}

} // namespace Msp
