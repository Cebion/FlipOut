#ifndef MSP_STRINGCODEC_CODECUTILS_H_
#define MSP_STRINGCODEC_CODECUTILS_H_

namespace Msp {
namespace StringCodec {

int transform_mapping_or_direct(const int *mapping, unsigned map_size, int ch, bool reverse);

} // namespace StringCodec
} // namespace Msp

#endif
