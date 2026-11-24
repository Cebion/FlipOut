#include <dlfcn.h>
#include "module.h"
#include "module_private.h"
#include "systemerror.h"

using namespace std;

namespace Msp {

Module::Module(const string &name)
{
	ModuleHandle handle = dlopen(name.c_str(), RTLD_NOW);
	if(!handle)
		throw system_error("dlopen", dlerror());

	priv = new Private;
	priv->handle = handle;
}

Module::~Module()
{
	dlclose(priv->handle);
	delete priv;
}

void *Module::get_symbol(const string &name) const
{
	dlerror();
	void *result = dlsym(priv->handle, name.c_str());
	if(!result)
	{
		const char *err = dlerror();
		if(err)
			throw system_error("dlsym", err);
	}

	return result;
}

} // namespace Msp
