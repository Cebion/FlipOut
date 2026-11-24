#ifndef MSP_FS_FILEMONITOR_PLATFORM_H_
#define MSP_FS_FILEMONITOR_PLATFORM_H_

namespace Msp {
namespace FS {

class INotify: public IO::EventObject
{
private:
	IO::Handle fd;

public:
	INotify();
	~INotify();

	int add_watch(const FS::Path &, int);
	void remove_watch(int);

	virtual void set_block(bool) { }
	virtual void set_inherit(bool) { }

protected:
	virtual std::size_t do_write(const char *, std::size_t);
	virtual std::size_t do_read(char *, std::size_t);

public:
	virtual const IO::Handle &get_handle(IO::Mode) { return fd; }
	virtual const IO::Handle &get_event_handle() { return fd; }
};


struct FileMonitor::Private
{
	FileMonitor &monitor;
	INotify inotify;

	Private(FileMonitor &);

	void events_available();
};

} // namespace FS
} // namespace Msp

#endif
