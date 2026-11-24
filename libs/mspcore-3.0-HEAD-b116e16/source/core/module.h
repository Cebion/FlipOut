#ifndef MSP_CORE_MODULE_H_
#define MSP_CORE_MODULE_H_

#include <string>
#include "noncopyable.h"

namespace Msp {

class Module: private NonCopyable
{
private:
	struct Private;

	Private *priv;

public:
	Module(const std::string &);
	~Module();

	void *get_symbol(const std::string &) const;
};

} // namespace Msp

#endif
