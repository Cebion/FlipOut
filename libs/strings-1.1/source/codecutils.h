/* $Id$

This file is part of libmspstrings
Copyright © 2006-2007 Mikko Rasa
Distributed under the LGPL
*/

#ifndef MSP_STRINGS_CODECUTILS_H_
#define MSP_STRINGS_CODECUTILS_H_

namespace Msp {
namespace Codecs {

int transform_mapping_or_direct(const int *mapping, unsigned map_size, int ch, bool reverse);

} // namespace Codecs
} // namespace Msp

#endif
