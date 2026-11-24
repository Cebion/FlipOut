#ifndef MSP_CORE_HASH_H_
#define MSP_CORE_HASH_H_

#include <cstdint>
#include <string>

namespace Msp {

/**
Computes a 32-bit Fowler-Noll-Vo (FNV-1a) hash.  The number of bits can be
limited to less than 32, in which case XOR-folding is used to reduce the hash
size.
*/
std::uint32_t hash32(const void *, unsigned, unsigned = 32);

/**
Convenience function to compute a 32-bit hash of a string.
*/
inline unsigned hash32(const std::string &s, unsigned b = 32)
{ return hash32(s.data(), s.size(), b); }

/**
Computes a 64-bit Fowler-Noll-Vo (FNV-1a) hash.  Note that even if bits is
limited to 32 or less, this does not produce the same result as hash32.
*/
std::uint64_t hash64(const void *, unsigned, unsigned = 64);

/**
Convenience function to compute a 64-bit hash of a string.
*/
inline std::uint64_t hash64(const std::string &s, unsigned b = 64)
{ return hash64(s.data(), s.size(), b); }

inline std::uint32_t fold32(std::uint64_t hash)
{ return hash^(hash>>32); }

inline std::uint16_t fold16(std::uint64_t hash)
{ return hash^(hash>>16)^(hash>>32)^(hash>>48); }

} // namespace Msp

#endif
