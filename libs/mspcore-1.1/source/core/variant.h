/* $Id: variant.h 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore
Copyright © 2008 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_CORE_VARIANT_H_
#define MSP_CORE_VARIANT_H_

#include "except.h"
#include "meta.h"

namespace Msp {

class Variant
{
private:
	struct StoreBase
	{
		virtual ~StoreBase() { }
		virtual StoreBase *clone() const =0;
	};

	template<typename T>
	struct Store: public StoreBase
	{
		T data;

		Store(T d): data(d) { }
		virtual StoreBase *clone() const { return new Store<T>(data); }
	};

	StoreBase *store;

public:
	Variant(): store(0) { }
	template<typename T>
	Variant(const T &v): store(new Store<typename RemoveConst<T>::Type>(v)) { }
	Variant(const Variant &v): store(v.store ? v.store->clone() : 0) { }
	~Variant() { delete store; }

	template<typename T>
	Variant &operator=(const T &v)
	{
		delete store;
		store=new Store<typename RemoveConst<T>::Type>(v);
		return *this;
	}

	Variant &operator=(const Variant &v)
	{
		delete store;
		store=(v.store ? v.store->clone() : 0);
		return *this;
	}

	template<typename T>
	T &value() const
	{
		typedef typename RemoveConst<T>::Type NCT;
		Store<NCT> *s=dynamic_cast<Store<NCT> *>(store);
		if(!s)
			throw InvalidState("Type mismatch");
		return s->data;
	}

	template<typename T>
	bool check_type() const
	{
		return dynamic_cast<Store<typename RemoveConst<T>::Type> *>(store)!=0;
	}

	template<typename T>
	operator T() const
	{ return value<T>(); }
};

} // namespace Msp

#endif
