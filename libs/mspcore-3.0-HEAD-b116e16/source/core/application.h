#ifndef MSP_CORE_APPLICATION_H_
#define MSP_CORE_APPLICATION_H_

#include <stdexcept>
#include <string>
#include "noncopyable.h"

namespace Msp {

/**
Base class for applications.  See also RegisteredApplication.
*/
class Application: private NonCopyable
{
protected:
	class Starter
	{
	protected:
		Starter();
	public:
		virtual ~Starter() { }

		virtual Application *create_app(int, char **) = 0;
	};

	bool done;
	int exit_code;

private:
	static Starter *starter_;
	static Application *app_;
	static const char *argv0_;
	static std::string name_;
	static void *data_;

	Application(const Application &);
	Application &operator=(const Application &);
protected:
	Application(const std::string & = std::string());
public:
	virtual ~Application() { }

	/** Constructs an instance of the registered application class and runs it.
	If the application throws a usage_error, a help message is printed.  The
	GetOpt class will throw such exceptions automatically in error conditions.

	This function can only be called once.  The global main() function provided
	by the library normally does it automatically at program startup. */
	static int run(int, char **, void * = 0, void (*)(void *) = 0);

	/** Sets application startup info, including argv[0] value and platform-
	specific data.

	This function can only be called once, and is normally called by
	Application::run(). */
	static void set_startup_info(const char *, void *);

	static void *get_data() { return data_; }
	static const char *get_argv0() { return argv0_; }
	static const std::string &get_name() { return name_; }

protected:
	/** Default main loop.  Calls tick() repeatedly until exit() is called.  A
	custom main loop should monitor the done member variable and return
	exit_code. */
	virtual int main();

	/** Sets the specified signal to be delivered to the sighandler member
	function. */
	void catch_signal(int);

	/** Causes the application to exit gracefully with the given exit code. */
	void exit(int);

	virtual void tick() { }
	virtual void sighandler(int) { }

private:
	/** Static wrapper function to call a member function of the Application
	instance. */
	static void sighandler_(int);
};


/**
Registers the class to be used for program startup.  The main application class
should be derived from this.
*/
template<typename T>
class RegisteredApplication: public Application
{
private:
	class Starter: public Application::Starter
	{
	public:
		Application *create_app(int argc, char **argv) { return new T(argc, argv); }
	};

	static Starter starter_;

protected:
	RegisteredApplication(const std::string &n = std::string()):
		Application(n)
	{ (void)starter_; }  // Force the starter into existence
};

template<typename T>
typename RegisteredApplication<T>::Starter RegisteredApplication<T>::starter_;

} // namespace Msp

#endif
