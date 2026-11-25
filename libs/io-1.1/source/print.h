/* $Id$

This file is part of libmspio
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_IO_PRINT_H_
#define MSP_IO_PRINT_H_

#include <msp/strings/formatter.h>
#include "base.h"
#include "console.h"

namespace Msp {
namespace IO {

/**
Writes a string to an I/O object.  Same as o.write(f).  Provided for
completeness with the other print functions.
*/
inline unsigned print(Base &o, const std::string &f)
{ return o.write(f); }

template<typename A1>
inline unsigned print(Base &o, const std::string &f, A1 a1)
{ return print(o, format(f, a1)); }

template<typename A1, typename A2>
inline unsigned print(Base &o, const std::string &f, A1 a1, A2 a2)
{ return print(o, format(f, a1, a2)); }

template<typename A1, typename A2, typename A3>
inline unsigned print(Base &o, const std::string &f, A1 a1, A2 a2, A3 a3)
{ return print(o, format(f, a1, a2, a3)); }

template<typename A1, typename A2, typename A3, typename A4>
inline unsigned print(Base &o, const std::string &f, A1 a1, A2 a2, A3 a3, A4 a4)
{ return print(o, format(f, a1, a2, a3, a4)); }

template<typename A1, typename A2, typename A3, typename A4, typename A5>
inline unsigned print(Base &o, const std::string &f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{ return print(o, format(f, a1, a2, a3, a4, a5)); }

/* The following functions print to console */

inline unsigned print(const std::string &f)
{ return print(cout, f); }

template<typename A1>
inline unsigned print(const std::string &f, A1 a1)
{ return print(cout, f, a1); }

template<typename A1, typename A2>
inline unsigned print(const std::string &f, A1 a1, A2 a2)
{ return print(cout, f, a1, a2); }

template<typename A1, typename A2, typename A3>
inline unsigned print(const std::string &f, A1 a1, A2 a2, A3 a3)
{ return print(cout, f, a1, a2, a3); }

template<typename A1, typename A2, typename A3, typename A4>
inline unsigned print(const std::string &f, A1 a1, A2 a2, A3 a3, A4 a4)
{ return print(cout, f, a1, a2, a3, a4); }

template<typename A1, typename A2, typename A3, typename A4, typename A5>
inline unsigned print(const std::string &f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{ return print(cout, f, a1, a2, a3, a4, a5); }

} // namespace IO
} // namespace Msp

#endif
