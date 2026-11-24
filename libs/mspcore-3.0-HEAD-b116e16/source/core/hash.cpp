#include <stdexcept>
#include "hash.h"

using namespace std;

namespace Msp {

/*
http://en.wikipedia.org/wiki/Fowler-Noll-Vo_hash_function
http://www.isthe.com/chongo/tech/comp/fnv/index.html
*/

uint32_t hash32(const void *data, unsigned size, unsigned bits)
{
	if(bits==0 || bits>32)
		throw invalid_argument("hash32");

	static const unsigned prime = 16777619;
	static const unsigned offset = 2166136261U;

	unsigned result = offset;
	for(unsigned i=0; i<size; ++i)
		result = (result^*(reinterpret_cast<const unsigned char *>(data)+i))*prime;

	if(bits<32)
		result = (result>>bits ^ result) & ((1<<bits)-1);

	return result;
}

uint64_t hash64(const void *data, unsigned size, unsigned bits)
{
	if(bits==0 || bits>64)
		throw invalid_argument("hash64");

	static const uint64_t prime = 1099511628211ULL;
	static const uint64_t offset = 14695981039346656037ULL;

	uint64_t result = offset;
	for(unsigned i=0; i<size; ++i)
		result = (result^*(reinterpret_cast<const unsigned char *>(data)+i))*prime;

	if(bits<64)
		result = (result>>bits ^ result) & ((1ULL<<bits)-1);

	return result;
}

} // namespace Msp
