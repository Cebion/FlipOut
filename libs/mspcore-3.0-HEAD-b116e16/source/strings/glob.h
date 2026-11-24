#ifndef MSP_STRINGS_GLOB_H_
#define MSP_STRINGS_GLOB_H_

#include <string>

namespace Msp {

bool globmatch(const std::string &, const std::string &);
bool globcasematch(const std::string &, const std::string &);

} // namespace Msp

#endif
