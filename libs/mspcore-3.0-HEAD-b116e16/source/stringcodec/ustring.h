#ifndef MSP_STRINGCODEC_USTRING_H_
#define MSP_STRINGCODEC_USTRING_H_

#include <string>

namespace Msp {
namespace StringCodec {

typedef int unichar;

typedef std::basic_string<unichar> ustring;

inline bool is_valid_unichar(unichar ch)
{ return ch>=0 && ch<=0x10FFFF && (ch<0xD800 || ch>0xDFFF) && (ch&0xFFFE)!=0xFFFE; }

} // namespace StringCodec
} // namespace Msp

#endif
