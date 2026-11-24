#include "filemonitor.h"
#include "filemonitor_platform.h"

using namespace std;

namespace Msp {
namespace FS {

void FileMonitor::platform_use_event_dispatcher()
{ }

void FileMonitor::prepare_file(MonitoredFile &)
{ }

void FileMonitor::cleanup_file(MonitoredFile &)
{ }

void FileMonitor::tick()
{ }


FileMonitor::Private::Private(FileMonitor &)
{
	throw logic_error("not implemented");
}

} // namespace FS
} // namespace Msp
