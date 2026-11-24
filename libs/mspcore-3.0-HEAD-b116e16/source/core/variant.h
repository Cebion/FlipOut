#ifndef MSP_CORE_VARIANT_H_
#define MSP_CORE_VARIANT_H_

#include <stdexcept>
#include <type_traits>
#include <typeinfo>
#include "meta.h"

namespace Msp {

class type_mismatch: public std::runtime_error
{
public:
	type_mismatch(const std::type_info &, const std::type_info &);
	~type_mismatch() throw() { }
};


class Variant
{
private:
	struct StoreBase
	{
		virtual ~StoreBase() { }

		virtual const std::type_info &type_id() const = 0;
		virtual StoreBase *clone() const = 0;
		virtual bool type_equals(const StoreBase &) const = 0;
		virtual bool value_equals(const StoreBase &) const = 0;
	};

	template<typename T>
	struct Store: public StoreBase
	{
		T data;

		Store(const T &d): data(d) { }

		virtual const std::type_info &type_id() const { return typeid(T); }
		virtual StoreBase *clone() const { return new Store<T>(data); }
		virtual bool type_equals(const StoreBase &s) const { return dynamic_cast<const Store<T> *>(&s); }
		virtual bool value_equals(const StoreBase &s) const { return value_equals_<T>(s); }

		template<typename U>
		typename std::enable_if<IsEqualityComparable<U>::value, bool>::type value_equals_(const StoreBase &s) const
		{ const Store<T> *t = dynamic_cast<const Store<T> *>(&s); return (t && t->data==data); }

		template<typename U>
		typename std::enable_if<!IsEqualityComparable<U>::value, bool>::type value_equals_(const StoreBase &) const
		{ return false; }
	};

	StoreBase *store;

public:
	Variant(): store(0) { }
	template<typename T>
	Variant(const T &v): store(new Store<typename std::remove_cv<T>::type>(v)) { }
	Variant(const Variant &v): store(v.store ? v.store->clone() : 0) { }
	~Variant() { delete store; }

	template<typename T>
	Variant &operator=(const T &v)
	{
		delete store;
		store = new Store<typename std::remove_cv<T>::type>(v);
		return *this;
	}

	Variant &operator=(const Variant &v)
	{
		delete store;
		store = (v.store ? v.store->clone() : 0);
		return *this;
	}

private:
	template<typename T>
	Store<typename std::remove_cv<T>::type> *get_typed_store() const
	{
		typedef typename std::remove_cv<T>::type NCT;
		Store<NCT> *s = dynamic_cast<Store<NCT> *>(store);
		if(!s)
			throw type_mismatch(typeid(T), (store ? store->type_id() : typeid(void)));
		return s;
	}

public:
	template<typename T>
	T &value()
	{
		return get_typed_store<T>()->data;
	}

	template<typename T>
	const T &value() const
	{
		return get_typed_store<T>()->data;
	}

	template<typename T>
	bool check_type() const
	{
		return dynamic_cast<Store<typename std::remove_cv<T>::type> *>(store)!=0;
	}

	bool check_same_type(const Variant &v) const
	{ return store && v.store && store->type_equals(*v.store); }

	bool operator==(const Variant &v) const
	{ return store && v.store && store->value_equals(*v.store); }

	bool operator!=(const Variant &v) const
	{ return !(operator==(v)); }

	template<typename T>
	operator T() const
	{ return value<T>(); }
};

} // namespace Msp

#endif
