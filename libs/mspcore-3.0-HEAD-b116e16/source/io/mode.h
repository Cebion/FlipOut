#ifndef MSP_IO_MODE_H_
#define MSP_IO_MODE_H_

#include <stdexcept>

namespace Msp {
namespace IO {

enum Mode
{
	M_NONE = 0,
	M_READ = 1,
	M_WRITE = 2,
	M_RDWR = M_READ|M_WRITE,
	M_APPEND = 4,
	M_NONBLOCK = 8,
	M_INHERIT = 16
};

inline Mode operator|(Mode m, Mode n)
{ return Mode(static_cast<int>(m)|static_cast<int>(n)); }

inline Mode operator&(Mode m, Mode n)
{ return Mode(static_cast<int>(m)&static_cast<int>(n)); }

inline Mode operator~(Mode m)
{ return Mode(~static_cast<int>(m)); }

inline void adjust_mode(Mode &m, Mode f, bool b)
{ m = b ? (m|f) : (m&~f); }


class invalid_access: public std::logic_error
{
public:
	invalid_access(Mode);
	~invalid_access() throw() { }
};

} // namespace IO
} // namespace Msp

#endif
