/* $Id: refptr.h 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore
Copyright © 2006-2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_CORE_REFPTR_H_
#define MSP_CORE_REFPTR_H_

namespace Msp {

/**
A reference counting smart pointer.  When the last RefPtr for the data gets
destroyed, the data is deleted as well.
*/
template<typename T>
class RefPtr
{
public:
	RefPtr(): data(0), count(0) { }
	RefPtr(T *d): data(d), count(0) { if(data) count=new unsigned(1); }
	RefPtr(const RefPtr &p): data(p.data), count(p.count) { incref(); }
	RefPtr &operator=(const RefPtr &p)
	{
		decref();
		data=p.data;
		count=p.count;
		incref();
		return *this;
	}
	
	~RefPtr() { decref(); }

	/**
	Makes the RefPtr release its reference of the data.  Note that if there are
	other RefPtrs left with the same data, it might still get deleted
	automatically.
	*/
	T *release()
	{
		T *d=data;
		data=0;
		decref();
		count=0;
		return d;
	}

	T *get() const        { return data; }
	T &operator*() const  { return *data; }
	T *operator->() const { return data; }
	operator bool() const { return data!=0; }

	template<typename U>
	static RefPtr<T> cast_dynamic(const RefPtr<U> &p)  { return RefPtr<T>(dynamic_cast<T *>(p.data), p.count); }
	template<typename U> friend class RefPtr;
private:
	T        *data;
	unsigned *count;

	RefPtr(T *d, unsigned *c): data(d), count(c) { incref(); }

	void  incref()
	{
		if(!count) return;
		++*count;
	}

	void  decref()
	{
		if(!count) return;
		--*count;
		if(!*count)
		{
			delete data;
			delete count;
			data=0;
			count=0;
		}
	}
};

} // namespace Msp

#endif
