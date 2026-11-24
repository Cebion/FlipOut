/* $Id: application.h 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore
Copyright © 2006 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#ifndef MSP_CORE_APPLICATION_H_
#define MSP_CORE_APPLICATION_H_

#include "semaphore.h"

namespace Msp {

/**
Base class for applications.  Inherit the main class from this and add a static
member of type RegApp<MainClass>.
*/
class Application
{
protected:
	enum LoopMode
	{
		NONE,       /// No main loop - main() will just return
		SLEEP,      /// Only sleep in the main loop - useful for threaded servers
		TICK_SLEEP, /// Call tick every iteration, with a short sleep in between
		TICK_YIELD, /// Call tick every iteration, with sched_yield in between
		TICK_BUSY   /// Call tick every iteration
	};

	class RegBase
	{
	public:
		virtual Application *create_app(int, char **)=0;
		virtual void usage(const char *, const char *, bool)=0;
		virtual ~RegBase() { }
	protected:
		RegBase();
	};

	template<typename T>
	class RegApp: public RegBase
	{
	public:
		Application *create_app(int argc, char **argv) { return new T(argc, argv); }
		void usage(const char *r, const char *a, bool b) { T::usage(r, a, b); }
	};

	bool done;
	int exit_code;

private:
	LoopMode loop_mode_;
	Semaphore sleep_sem_;

	static RegBase *reg_app_;
	static Application *app_;
	static void *data_;

protected:
	Application();
public:
	virtual ~Application() { }

	static int run(int, char **, void * =0);
	static void usage(const char *, const char *, bool);
	static void *get_data() { return data_; }

protected:
	virtual int main();
	void catch_signal(int);
	void set_loop_mode(LoopMode);
	void induce_tick();
	void exit(int);
	virtual void tick() { }
	virtual void sighandler(int) { }
private:
	static void sighandler_(int);

	Application(const Application &);
	Application &operator=(const Application &);
};

} // namespace Msp

#endif
