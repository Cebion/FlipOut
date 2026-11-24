#include <msp/strings/format.h>
#include "seekable.h"

using namespace std;

namespace Msp {
namespace IO {

bad_seek::bad_seek(SeekOffset offset, SeekType type):
	runtime_error(type==S_BEG ? lexical_cast<string>(offset) :
		type==S_CUR ? format("CUR%+d", offset) :
		type==S_END ? format("END%+d", offset) :
		format("SeekType(%d)", type))
{ }

} // namespace IO
} // namespace Msp
