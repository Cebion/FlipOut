#include "module.h"
#include "module_private.h"
#include "systemerror.h"

using namespace std;

namespace Msp {

Module::Module(const string &name)
{
	ModuleHandle handle = LoadLibrary(name.c_str());
	if(!handle)
		throw system_error("LoadLibrary");

	priv = new Private;
	priv->handle = handle;
}

Module::~Module()
{
	FreeLibrary(priv->handle);
	delete priv;
}

void *Module::get_symbol(const string &name) const
{
	FARPROC result = GetProcAddress(priv->handle, name.c_str());
	if(!result)
		throw system_error("GetProcAddress");

	return reinterpret_cast<void *>(result);
}

} // namespace Msp
