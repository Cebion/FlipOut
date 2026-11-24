#ifndef MSP_CORE_TYPEREGISTRY_H_
#define MSP_CORE_TYPEREGISTRY_H_

#include <map>
#include <string>
#include "maputils.h"
#include "noncopyable.h"

namespace Msp {

/**
Associates types with keywords for invoking an action.  The action must be
a template struct or class with operator() taking two parameters.  The first
parameter is the keyword of the type and the second is data passed in to the
invoke function.  The type of the data is specified as the second template
parameter to TypeRegistry.  Note that the type is used as is; if you want to
pass a reference, it must be explicit in the template argument.
*/
template<template<typename> class A, typename T>
class TypeRegistry: private NonCopyable
{
private:
	class TypeBase
	{
	protected:
		std::string keyword;

		TypeBase(const std::string &kw): keyword(kw) { }
	public:
		virtual ~TypeBase() { }

		virtual void invoke(T) const = 0;
	};

	template<typename R>
	class RegisteredType: public TypeBase
	{
	public:
		A<R> action;

		RegisteredType(const std::string &kw): TypeBase(kw) { }

		virtual void invoke(T arg) const { action(this->keyword, arg); }
	};

	std::map<std::string, TypeBase *> types;

public:
	~TypeRegistry();

	/** Registers a type. */
	template<typename R>
	void register_type(const std::string &);

	/** Invokes the action for a specific type, which must have been registered
	before. */
	void invoke(const std::string &, T) const;

	/** Invokes the action for all registered types. */
	void invoke_all(T) const;
};

template<template<typename> class A, typename T>
TypeRegistry<A, T>::~TypeRegistry()
{
	for(auto &kvp: types)
		delete kvp.second;
}

template<template<typename> class A, typename T>
template<typename R>
void TypeRegistry<A, T>::register_type(const std::string &kw)
{
	if(types.count(kw))
		throw key_error(kw);

	types[kw] = new RegisteredType<R>(kw);
}

template<template<typename> class A, typename T>
void TypeRegistry<A, T>::invoke(const std::string &kw, T arg) const
{
	get_item(types, kw)->invoke(arg);
}

template<template<typename> class A, typename T>
void TypeRegistry<A, T>::invoke_all(T arg) const
{
	for(auto &kvp: types)
		kvp.second->invoke(arg);
}

} // namespace Msp

#endif
