#ifndef MSP_IO_PRINT_H_
#define MSP_IO_PRINT_H_

#include <msp/strings/format.h>
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

template<typename... Args>
inline unsigned print(Base &o, const std::string &f, Args... args)
{ return print(o, format(f, args...)); }

/* The following functions print to console */

inline unsigned print(const std::string &f)
{ return print(cout, f); }

template<typename... Args>
inline unsigned print(const std::string &f, Args... args)
{ return print(cout, f, args...); }

} // namespace IO
} // namespace Msp

#endif
