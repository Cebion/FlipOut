/* $Id$

This file is part of libmspio
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#ifndef MSP_IO_UTILS_H_
#define MSP_IO_UTILS_H_

namespace Msp {
namespace IO {

class Base;

/**
Reads data from an object.  Does not return until the requested amount of data
is read, regardless of the blocking mode of the object.

Note: If the data is not immediately available and the object is in non-blocking
mode, this function effectively becomes a busyloop until it can get more data.
*/
unsigned read_all(Base &, char *, unsigned);

} // namespace IO
} // namespace Msp

#endif
