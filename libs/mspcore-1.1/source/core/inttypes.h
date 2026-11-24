#ifndef MSP_CORE_INTTYPES_H_
#define MSP_CORE_INTTYPES_H_

namespace Msp {

template<typename T, unsigned S, bool C=(sizeof(typename T::Head)==S/8)>
struct IntChooser: TypeChooser<T, S, C>
{ }

typedef TypeList<char, TypeList<short, TypeList<int, TypeList<long, TypeList<long long, Null> > > > >
	IntTypes;

template<unsigned S>
struct Int
{
	typedef typename IntChooser<IntTypes, S>::Type Type;
};

typedef Int<8>::Type Int8;
typedef Int<16>::Type Int16;
typedef Int<32>::Type Int32;
typedef Int<64>::Type Int64;

typedef unsigned Int<8>::Type UInt8;
typedef unsigned Int<16>::Type UInt16;
typedef unsigned Int<32>::Type UInt32;
typedef unsigned Int<64>::Type UInt64;

} // namespace Msp

#endif
