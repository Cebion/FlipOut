#include "datetime.h"
#include "timestamp.h"
#include "utils.h"

using namespace std;

namespace Msp {
namespace Time {

string format_now(const string &fmt)
{
	return DateTime(now()).format(fmt);
}

} // namespace Time
} // namespace Msp
