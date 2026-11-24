#ifndef MSP_CORE_META_H_
#define MSP_CORE_META_H_

#include <cstddef>
#include "attributes.h"

namespace Msp {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
template<typename T>
struct DEPRECATED RemoveConst
{ typedef T Type; };

template<typename T>
struct DEPRECATED RemoveConst<const T>
{ typedef T Type; };


template<typename T>
struct DEPRECATED RemoveReference
{ typedef T Type; };

template<typename T>
struct DEPRECATED RemoveReference<T &>
{ typedef T Type; };


template<typename T>
struct DEPRECATED RemoveConstReference
{ typedef typename RemoveConst<typename RemoveReference<T>::Type>::Type Type; };


template<bool c, typename R>
struct DEPRECATED EnableIf;

template<typename R>
struct DEPRECATED EnableIf<true, R>
{ typedef R Yes; };

template<typename R>
struct DEPRECATED EnableIf<false, R>
{ typedef R No; };
#pragma GCC diagnostic pop


/**
Common fragments used in SFINAE-based decider constructs.
*/
struct Sfinae
{
	struct Yes { char c[2]; };
	struct No { char c; };

	template<typename T>
	static No f(...);

	template<typename C, typename T>
	struct Evaluate
	{
		enum { value = (sizeof(C::template f<T>(0))==sizeof(Yes)) };
	};
};


struct CheckEqualityComparable: Sfinae
{
	static int &v;
	template<typename T>
	static Yes f(int (*)[sizeof(reinterpret_cast<const T &>(v)==reinterpret_cast<const T &>(v))]);
	using Sfinae::f;
};

template<typename T>
struct IsEqualityComparable: Sfinae::Evaluate<CheckEqualityComparable, T> { };

} // namespace Msp

#endif
