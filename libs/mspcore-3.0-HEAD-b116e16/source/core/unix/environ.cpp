#include <cstdlib>
#include "environ.h"
#include "mutex.h"

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
	const char *value = std::getenv(name.c_str());
	return (value ? string(value) : string());
}

void setenv(const string &name, const string &value)
{
	MutexLock _lock(env_mutex());
	::setenv(name.c_str(), value.c_str(), true);
}

void unsetenv(const string &name)
{
	MutexLock _lock(env_mutex());
	::unsetenv(name.c_str());
}

}
