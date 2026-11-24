#ifndef FILEMONITOR_H_
#define FILEMONITOR_H_

#include <msp/core/noncopyable.h>
#include <msp/fs/path.h>
#include <msp/io/eventdispatcher.h>
#include <msp/io/eventobject.h>

namespace Msp {
namespace FS {

class FileMonitor: NonCopyable
{
private:
	struct Private;

	struct MonitoredFile
	{
		FS::Path path;
		bool modified;
		int tag;
	};

public:
	sigc::signal<void, const FS::Path &> signal_file_modified;

private:
	Private *priv;
	IO::EventDispatcher *event_disp;
	std::vector<MonitoredFile> files;

public:
	FileMonitor();
	~FileMonitor();

	void add_file(const FS::Path &);
	void remove_file(const FS::Path &);
private:
	void prepare_file(MonitoredFile &);
	void cleanup_file(MonitoredFile &);

public:
	void use_event_dispatcher(IO::EventDispatcher &);
private:
	void platform_use_event_dispatcher();

public:
	void tick();
	void tick(const Msp::Time::TimeDelta &);
};

} // namespace FS
} // namespace Msp

#endif
