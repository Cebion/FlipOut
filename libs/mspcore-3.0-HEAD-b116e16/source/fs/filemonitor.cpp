#include <msp/core/algorithm.h>
#include "filemonitor.h"
#include "filemonitor_platform.h"

using namespace std;

namespace Msp {
namespace FS {

FileMonitor::FileMonitor():
	priv(new Private(*this)),
	event_disp(0)
{ }

FileMonitor::~FileMonitor()
{
	delete priv;
}

void FileMonitor::use_event_dispatcher(IO::EventDispatcher &ed)
{
	if(event_disp)
		throw logic_error("event_disp!=0");

	event_disp = &ed;
	platform_use_event_dispatcher();
}

void FileMonitor::add_file(const FS::Path &path)
{
	MonitoredFile file;
	file.path = path;
	file.modified = false;
	prepare_file(file);
	files.push_back(file);
}

void FileMonitor::remove_file(const FS::Path &path)
{
	auto i = find_member(files, path, &MonitoredFile::path);
	if(i!=files.end())
	{
		cleanup_file(*i);
		if(&*i!=&files.back())
			*i = files.back();
		files.pop_back();
	}
}

} // namespace FS
} // namespace Msp
