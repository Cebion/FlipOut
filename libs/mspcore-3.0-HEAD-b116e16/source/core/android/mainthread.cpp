#include <cstring>
#include <cstdlib>
#include <android/window.h>
#include <msp/fs/dir.h>
#include <msp/fs/path.h>
#include <msp/fs/utils.h>
#include "application.h"
#include "errorlogger.h"
#include "mainthread.h"

using namespace std;

namespace Msp {
namespace Android {

MainThread::MainThread(ANativeActivity *a):
	asset_manager(a->assetManager),
	int_data_path(a->internalDataPath),
	starting_up(true)
{
	attach_to_activity(a);
	startup_mutex.lock();
	launch();
}

void MainThread::attach_to_activity(ANativeActivity *a)
{
	activity = a;
	activity->callbacks->onDestroy = &activity_destroyed;
	activity->callbacks->onNativeWindowCreated = &native_window_created;
	activity->callbacks->onNativeWindowResized = &native_window_resized;
	activity->callbacks->onNativeWindowDestroyed = &native_window_destroyed;
	activity->callbacks->onInputQueueCreated = &input_queue_created;
	activity->callbacks->onInputQueueDestroyed = &input_queue_destroyed;
	activity->instance = this;

	set_window_flags(AWINDOW_FLAG_FULLSCREEN, 0);
}

void MainThread::wait_for_app_created()
{
	MutexLock lock(startup_mutex);
}

void MainThread::resume_startup()
{
	if(starting_up)
	{
		starting_up = false;
		startup_mutex.unlock();
	}
}

JavaVM *MainThread::get_java_vm() const
{
	if(!activity)
		return 0;

	return activity->vm;
}

void MainThread::set_window_flags(unsigned set, unsigned clear)
{
	ANativeActivity_setWindowFlags(activity, set, clear);
}

void MainThread::main()
{
	/* I have no idea how dependable this is, but it seems to be the only way
	to get the package name aside from making a Java call through JNI */
	char *appname = strdup(int_data_path[-2].c_str());
	char *argv[] = { appname, 0 };
	Msp::Android::ErrorLogger err_logger;
	FS::chdir(FS::dirname(int_data_path));
	Msp::Application::run(1, argv, this, &app_created);
	free(appname);
	if(activity)
	{
		activity->callbacks->onDestroy = &end_process;
		ANativeActivity_finish(activity);
	}
	else
		end_process(0);
}

void MainThread::app_created(void *data)
{
	reinterpret_cast<MainThread *>(data)->resume_startup();
}

void MainThread::activity_destroyed(ANativeActivity *activity)
{
	reinterpret_cast<MainThread *>(activity->instance)->activity = 0;
}

void MainThread::native_window_created(ANativeActivity *activity, ANativeWindow *window)
{
	reinterpret_cast<MainThread *>(activity->instance)->signal_native_window_created.emit(window);
}

void MainThread::native_window_resized(ANativeActivity *activity, ANativeWindow *window)
{
	reinterpret_cast<MainThread *>(activity->instance)->signal_native_window_resized.emit(window);
}

void MainThread::native_window_destroyed(ANativeActivity *activity, ANativeWindow *window)
{
	reinterpret_cast<MainThread *>(activity->instance)->signal_native_window_destroyed.emit(window);
}

void MainThread::input_queue_created(ANativeActivity *activity, AInputQueue *queue)
{
	reinterpret_cast<MainThread *>(activity->instance)->signal_input_queue_created.emit(queue);
}

void MainThread::input_queue_destroyed(ANativeActivity *activity, AInputQueue *queue)
{
	reinterpret_cast<MainThread *>(activity->instance)->signal_input_queue_destroyed.emit(queue);
}

void MainThread::end_process(ANativeActivity *)
{
	exit(0);
}

} // namespace Android
} // namespace Msp
