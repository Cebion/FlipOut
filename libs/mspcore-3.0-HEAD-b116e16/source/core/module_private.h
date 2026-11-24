#ifndef MSP_CORE_MODULE_PRIVATE_H_
#define MSP_CORE_MODULE_PRIVATE_H_

#include "module.h"
#include "module_platform.h"

namespace Msp {

struct Module::Private
{
	ModuleHandle handle;
};

} // namespace Msp

#endif
