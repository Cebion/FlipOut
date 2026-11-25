/* $Id$

This file is part of libmspio
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#ifndef MSP_IO_MODE_H_
#define MSP_IO_MODE_H_

namespace Msp {
namespace IO {

enum Mode
{
	M_NONE=0,
	M_READ=1,
	M_WRITE=2,
	M_RDWR=M_READ|M_WRITE,
	M_APPEND=4,
	M_NONBLOCK=8
};

inline Mode operator|(Mode m, Mode n)
{ return Mode(static_cast<int>(m)|static_cast<int>(n)); }

inline Mode operator&(Mode m, Mode n)
{ return Mode(static_cast<int>(m)&static_cast<int>(n)); }

inline Mode operator~(Mode m)
{ return Mode(~static_cast<int>(m)); }

} // namespace IO
} // namespace Msp

#endif
