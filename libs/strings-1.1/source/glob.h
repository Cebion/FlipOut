/* $Id$

This file is part of libmspstrings
Copyright © 2007 Mikko Rasa
Distributed under the LGPL
*/
#ifndef MSP_STRINGS_GLOB_H_
#define MSP_STRINGS_GLOB_H_

#include <string>

namespace Msp {

bool globmatch(const std::string &, const std::string &);
bool globcasematch(const std::string &, const std::string &);

} // namespace Msp

#endif
