#include <stdlib.h>
#include "environ.h"
#include "mutex.h"

#ifdef __GNUC__
extern "C" errno_t getenv_s(size_t *, char *, size_t, const char *);
#endif

using namespace std;

namespace Msp {

static Mutex &env_mutex()
{
	static Mutex mutex;
	return mutex;
}

string getenv(const string &name)
{
	MutexLock _lock(env_mutex());
	char buffer[1024];
	size_t result;
	if(!getenv_s(&result, buffer, sizeof(buffer), name.c_str()))
		return buffer;
	else
		return string();
}

void setenv(const string &name, const string &value)
{
	MutexLock _lock(env_mutex());
	_putenv_s(name.c_str(), value.c_str());
}

void unsetenv(const string &name)
{
	MutexLock _lock(env_mutex());
	_putenv_s(name.c_str(), "");
}

}
