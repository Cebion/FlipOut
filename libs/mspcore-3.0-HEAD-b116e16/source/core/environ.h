#ifndef ENVIRON_H_
#define ENVIRON_H_

#include <string>

namespace Msp {

std::string getenv(const std::string &);
void setenv(const std::string &, const std::string &);
void unsetenv(const std::string &);

} // namespace Msp

#endif
