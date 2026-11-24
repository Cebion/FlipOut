#ifndef MSP_CORE_MAPUTILS_H_
#define MSP_CORE_MAPUTILS_H_

#include <stdexcept>
#include <typeinfo>
#include <msp/strings/lexicalcast.h>

namespace Msp {

namespace MapUtilsInternal {

/* This dummy struct is used to introduce a conversion, making the overloaded
operator below worse than the templated one provided in lexicalcast.h. */
struct Any
{
	template<typename T>
	Any(const T &) { }
};

/* This must be hidden in the internal namespace to avoid interfering with
other things. */
inline void operator<<(LexicalConverter &, Any)
{ }

template<typename T>
static std::string stringify_key(const T &k)
{
	try
	{
		LexicalConverter conv((Fmt()));
		conv << k;
		return conv.get();
	}
	catch(const lexical_error &)
	{
		return "<cannot display value>";
	}
}

} // namespace Internal

class key_error: public std::runtime_error
{
public:
	template<typename T>
	key_error(const T &k):
		runtime_error(make_what(typeid(T), MapUtilsInternal::stringify_key(k)))
	{ }

	virtual ~key_error() throw() { }

private:
	static std::string make_what(const std::type_info &, const std::string &);
};


template<typename T>
typename T::mapped_type &get_item(T &map, const typename T::key_type &key)
{
	auto i = map.find(key);
	if(i==map.end())
		throw key_error(key);

	return i->second;
}

template<typename T>
const typename T::mapped_type &get_item(const T &map, const typename T::key_type &key)
{
	auto i = map.find(key);
	if(i==map.end())
		throw key_error(key);

	return i->second;
}

template<typename D, typename T>
D *get_item(const T &map, const typename T::key_type &key)
{
	return dynamic_cast<D *>(get_item(map, key));
}

template<typename T>
typename T::iterator insert_unique(T &map, const typename T::key_type &key, const typename T::mapped_type &item)
{
	if(map.count(key))
		throw key_error(key);

	return map.insert(std::make_pair(key, item)).first;
}

template<typename T>
void remove_existing(T &map, const typename T::key_type &key)
{
	if(!map.count(key))
		throw key_error(key);

	map.erase(key);
}

} // namespace Msp

#endif
