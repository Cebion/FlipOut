/* $Id$

This file is part of libmspio
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#ifndef MSP_IO_TYPES_H_
#define MSP_IO_TYPES_H_

#ifdef WIN32
#include <windows.h>
#endif

namespace Msp {
namespace IO {

#ifdef WIN32
typedef HANDLE Handle;
#define MSP_IO_INVALID_HANDLE INVALID_HANDLE_VALUE
#else
typedef int Handle;
#define MSP_IO_INVALID_HANDLE -1
#endif

} // namespace IO
} // namespace Msp

#endif
