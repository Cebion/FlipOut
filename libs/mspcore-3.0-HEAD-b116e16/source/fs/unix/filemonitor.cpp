#include <sys/inotify.h>
#include <linux/limits.h>
#include <msp/core/algorithm.h>
#include <msp/core/systemerror.h>
#include <msp/io/handle_private.h>
#include "filemonitor.h"
#include "filemonitor_platform.h"

using namespace std;

namespace Msp {
namespace FS {

INotify::INotify()
{
	*fd = inotify_init();
	mode = IO::M_READ;
	set_events(IO::P_INPUT);
}

INotify::~INotify()
{
	IO::sys_close(fd);
}

int INotify::add_watch(const FS::Path &path, int ev)
{
	int ret = inotify_add_watch(*fd, path.c_str(), ev);
	if(ret==-1)
		throw system_error("inotify_add_watch");
	return ret;
}

void INotify::remove_watch(int wd)
{
	int ret = inotify_rm_watch(*fd, wd);
	if(ret==-1)
		throw system_error("inotify_rm_watch");
}

size_t INotify::do_write(const char *, size_t)
{
	check_access(IO::M_WRITE);
	return 0;
}

size_t INotify::do_read(char *buf, size_t size)
{
	return IO::sys_read(fd, buf, size);
}


void FileMonitor::platform_use_event_dispatcher()
{
	event_disp->add(priv->inotify);
}

void FileMonitor::prepare_file(MonitoredFile &file)
{
	file.tag = priv->inotify.add_watch(file.path, IN_MODIFY|IN_CLOSE_WRITE|IN_DELETE_SELF);
}

void FileMonitor::cleanup_file(MonitoredFile &file)
{
	if(file.tag!=-1)
		priv->inotify.remove_watch(file.tag);
}

void FileMonitor::tick()
{
	bool first = true;
	while(1)
	{
		if(!first && !IO::poll(priv->inotify, IO::P_INPUT, Time::zero))
			break;

		first = false;
		priv->events_available();
	}
}

void FileMonitor::tick(const Time::TimeDelta &timeout)
{
	if(IO::poll(priv->inotify, IO::P_INPUT, timeout))
		tick();
}


FileMonitor::Private::Private(FileMonitor &m):
	monitor(m)
{
	inotify.signal_data_available.connect(sigc::mem_fun(this, &Private::events_available));
}

void FileMonitor::Private::events_available()
{
	vector<FS::Path> changed_files;
	char event_buf[sizeof(struct inotify_event)+NAME_MAX+1];
	unsigned len = inotify.read(event_buf, sizeof(event_buf));
	for(unsigned i=0; i<len; )
	{
		struct inotify_event *event = reinterpret_cast<struct inotify_event *>(event_buf+i);
		auto j = find_member(monitor.files, event->wd, &MonitoredFile::tag);
		if(j!=monitor.files.end())
		{
			if(event->mask&IN_MODIFY)
				j->modified = true;
			if(((event->mask&IN_CLOSE_WRITE) && j->modified) || (event->mask&IN_DELETE_SELF))
			{
				j->modified = false;
				changed_files.push_back(j->path);
			}
			if(event->mask&IN_IGNORED)
				j->tag = -1;
		}
		i += sizeof(struct inotify_event)+event->len;
	}

	for(const FS::Path &p: changed_files)
		monitor.signal_file_modified.emit(p);

	for(auto j=monitor.files.begin(); j!=monitor.files.end(); )
	{
		if(j->tag==-1)
			monitor.files.erase(j++);
		else
			++j;
	}
}

} // namespace FS
} // namespace Msp
