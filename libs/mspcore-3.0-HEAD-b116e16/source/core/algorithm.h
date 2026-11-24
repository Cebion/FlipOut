#ifndef MSP_CORE_ALGORITHM_H_
#define MSP_CORE_ALGORITHM_H_

#include <algorithm>

namespace Msp {

template<typename Container, typename T>
inline typename Container::iterator find(Container &cont, const T &value)
{
	return std::find(cont.begin(), cont.end(), value);
}

template<typename Container, typename T>
inline typename Container::const_iterator find(const Container &cont, const T &value)
{
	return std::find(cont.begin(), cont.end(), value);
}

template<typename Container, typename Predicate>
inline typename Container::iterator find_if(Container &cont, Predicate pred)
{
	return std::find_if(cont.begin(), cont.end(), pred);
}

template<typename Container, typename Predicate>
inline typename Container::const_iterator find_if(const Container &cont, Predicate pred)
{
	return std::find_if(cont.begin(), cont.end(), pred);
}

template<typename Container, typename T>
inline typename Container::iterator lower_bound(Container &cont, const T &value)
{
	return std::lower_bound(cont.begin(), cont.end(), value);
}

template<typename Container, typename T>
inline typename Container::const_iterator lower_bound(const Container &cont, const T &value)
{
	return std::lower_bound(cont.begin(), cont.end(), value);
}

template<typename Container, typename T, typename Predicate>
inline typename Container::iterator lower_bound(Container &cont, const T &value, Predicate pred)
{
	return std::lower_bound(cont.begin(), cont.end(), value, pred);
}

template<typename Container, typename T, typename Predicate>
inline typename Container::const_iterator lower_bound(const Container &cont, const T &value, Predicate pred)
{
	return std::lower_bound(cont.begin(), cont.end(), value, pred);
}

template<typename Container, typename T>
inline typename Container::iterator upper_bound(Container &cont, const T &value)
{
	return std::upper_bound(cont.begin(), cont.end(), value);
}

template<typename Container, typename T>
inline typename Container::const_iterator upper_bound(const Container &cont, const T &value)
{
	return std::upper_bound(cont.begin(), cont.end(), value);
}

template<typename Container, typename T, typename Predicate>
inline typename Container::iterator upper_bound(Container &cont, const T &value, Predicate pred)
{
	return std::upper_bound(cont.begin(), cont.end(), value, pred);
}

template<typename Container, typename T, typename Predicate>
inline typename Container::const_iterator upper_bound(const Container &cont, const T &value, Predicate pred)
{
	return std::upper_bound(cont.begin(), cont.end(), value, pred);
}

template<typename Container>
inline void sort(Container &cont)
{
	std::sort(cont.begin(), cont.end());
}

template<typename Container, typename Predicate>
inline void sort(Container &cont, Predicate pred)
{
	std::sort(cont.begin(), cont.end(), pred);
}

template<typename Container>
inline void stable_sort(Container &cont)
{
	std::stable_sort(cont.begin(), cont.end());
}

template<typename Container, typename Predicate>
inline void stable_sort(Container &cont, Predicate pred)
{
	std::stable_sort(cont.begin(), cont.end(), pred);
}

template<typename C, typename T>
struct MemberMatch
{
	const T &value;
	T C::*mem_ptr;

	MemberMatch(const T &v, T C::*p): value(v), mem_ptr(p) { }

	bool operator()(const C &obj) { return obj.*mem_ptr==value; }
};

template<typename Container, typename T>
inline typename Container::iterator find_member(Container &cont, const T &value, T Container::value_type::*mp)
{
	return find_if(cont, MemberMatch<typename Container::value_type, T>(value, mp));
}

template<typename Container, typename T>
inline typename Container::const_iterator find_member(const Container &cont, const T &value, T Container::value_type::*mp)
{
	return find_if(cont, MemberMatch<typename Container::value_type, T>(value, mp));
}

template<typename C, typename T>
struct MemberCompare
{
	T C::*mem_ptr;

	MemberCompare(T C::*p): mem_ptr(p) { }

	bool operator()(const C &obj, const T &v) { return obj.*mem_ptr<v; }
	bool operator()(const C &obj1, const C &obj2) { return obj1.*mem_ptr<obj2.*mem_ptr; }
};

template<typename Container, typename T>
inline typename Container::iterator lower_bound_member(Container &cont, const T &value, T Container::value_type::*mp)
{
	return lower_bound(cont, value, MemberCompare<typename Container::value_type, T>(mp));
}

template<typename Container, typename T>
inline typename Container::const_iterator lower_bound_member(const Container &cont, const T &value, T Container::value_type::*mp)
{
	return lower_bound(cont, value, MemberCompare<typename Container::value_type, T>(mp));
}

template<typename Container, typename T>
inline typename Container::iterator upper_bound_member(Container &cont, const T &value, T Container::value_type::*mp)
{
	return upper_bound(cont, value, MemberCompare<typename Container::value_type, T>(mp));
}

template<typename Container, typename T>
inline typename Container::const_iterator upper_bound_member(const Container &cont, const T &value, T Container::value_type::*mp)
{
	return upper_bound(cont, value, MemberCompare<typename Container::value_type, T>(mp));
}

template<typename Container, typename T>
inline void sort_member(Container &cont, T Container::value_type::*mp)
{
	sort(cont, MemberCompare<typename Container::value_type, T>(mp));
}

template<typename Container, typename T>
inline void stable_sort_member(Container &cont, T Container::value_type::*mp)
{
	stable_sort(cont, MemberCompare<typename Container::value_type, T>(mp));
}

template<typename Container, typename Predicate>
inline void transform(Container &cont, Predicate pred)
{
	transform(cont.begin(), cont.end(), cont.begin(), pred);
}

} // namespace Msp

#endif
