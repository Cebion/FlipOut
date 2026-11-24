#include "mode.h"

using namespace std;

namespace Msp {
namespace IO {

invalid_access::invalid_access(Mode m):
	logic_error(m==M_READ ? "read" :
		m==M_WRITE ? "write" :
		m==M_RDWR ? "read-write" :
		"generic")
{ }

} // namespace IO
} // namespace Msp
