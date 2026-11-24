#ifndef MSP_CORE_ANDROID_MAINTHREAD_H_
#define MSP_CORE_ANDROID_MAINTHREAD_H_

#include <android/native_activity.h>
#include <sigc++/signal.h>
#include <msp/fs/path.h>
#include "mutex.h"
#include "thread.h"

namespace Msp {
namespace Android {

class MainThread: public Thread
{
public:
	sigc::signal<void, ANativeWindow *> signal_native_window_created;
	sigc::signal<void, ANativeWindow *> signal_native_window_resized;
	sigc::signal<void, ANativeWindow *> signal_native_window_destroyed;
	sigc::signal<void, AInputQueue *> signal_input_queue_created;
	sigc::signal<void, AInputQueue *> signal_input_queue_destroyed;

private:
	ANativeActivity *activity;
	AAssetManager *asset_manager;
	FS::Path int_data_path;
	bool starting_up;
	Mutex startup_mutex;

public:
	MainThread(ANativeActivity *);

	void attach_to_activity(ANativeActivity *);
	bool is_starting_up() const { return starting_up; }
	void wait_for_app_created();
	void resume_startup();

	JavaVM *get_java_vm() const;
	AAssetManager *get_asset_manager() const { return asset_manager; }
	const FS::Path &get_internal_data_path() const { return int_data_path; }

	void set_window_flags(unsigned, unsigned);

private:
	virtual void main();

	static void app_created(void *);
	static void activity_destroyed(ANativeActivity *);
	static void native_window_created(ANativeActivity *, ANativeWindow *);
	static void native_window_resized(ANativeActivity *, ANativeWindow *);
	static void native_window_destroyed(ANativeActivity *, ANativeWindow *);
	static void input_queue_created(ANativeActivity *, AInputQueue *);
	static void input_queue_destroyed(ANativeActivity *, AInputQueue *);
	static void end_process(ANativeActivity *);
};

} // namespace Android
} // namespace Msp

#endif
