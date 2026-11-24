#ifndef MSP_CORE_REFPTR_H_
#define MSP_CORE_REFPTR_H_

namespace Msp {

struct RefCounts
{
	enum
	{
		KEEP = 1U<<(sizeof(unsigned)*8-1)
	};

	unsigned count;
	unsigned weak_count;

	RefCounts(): count(0), weak_count(0) { }
};

template<typename T>
class WeakPtr;


/**
A reference counting smart pointer.  When the last RefPtr for the data gets
destroyed, the data is deleted as well.
*/
template<typename T>
class RefPtr
{
	template<typename U> friend class RefPtr;
	template<typename U> friend class WeakPtr;

private:
	T *data;
	RefCounts *counts;

public:
	RefPtr(): data(0), counts(0) { }
	RefPtr(T *d): data(d), counts(data ? new RefCounts : 0) { incref(); }
private:
	RefPtr(T *d, RefCounts *c): data(d), counts(d ? c : 0) { incref(); }

public:
	/* Must have this or the compiler will generate a default copy-c'tor despite
	the template version */
	RefPtr(const RefPtr &p): data(p.data), counts(p.counts) { incref(); }

	template<typename U>
	RefPtr(const RefPtr<U> &p): data(p.data), counts(p.counts) { incref(); }

	template<typename U>
	RefPtr(const WeakPtr<U> &p): data(p.get()), counts(data ? p.counts : 0) { incref(); }

	~RefPtr() { decref(); }

	RefPtr &operator=(T *);

	// Likewise for the assignment operator
	RefPtr &operator=(const RefPtr &p) { return assign(p); }

	template<typename U>
	RefPtr &operator=(const RefPtr<U> &p) { return assign(p); }

	template<typename U>
	RefPtr &operator=(const WeakPtr<U> &p) { return assign(RefPtr(p)); }

private:
	template<typename U>
	RefPtr &assign(const RefPtr<U> &);

public:
	/** Makes the RefPtr release its reference of the data without deleting it.
	Note that if there are other RefPtrs left with the same data, it might
	still get deleted automatically. */
	T *release();

	/** Marks the data to not be deleted.  This affects all RefPtrs with the
	same data. */
	void keep() { if(counts) counts->count |= RefCounts::KEEP; }

	T *get() const { return data; }
	T &operator*() const { return *data; }
	T *operator->() const { return data; }
	operator bool() const { return data!=0; }

	unsigned refcount() const { return (data ? counts->count : 0); }

	template<typename U>
	static RefPtr<T> cast_dynamic(const RefPtr<U> &p)
	{ return RefPtr<T>(dynamic_cast<T *>(p.data), p.counts); }

private:
	void incref() { if(counts) ++counts->count; }
	void decref();
};


template<typename T>
class WeakPtr
{
	template<typename U> friend class RefPtr;
	template<typename U> friend class WeakPtr;

private:
	T *data;
	RefCounts *counts;

public:
	WeakPtr(): data(0), counts(0) { }
private:
	WeakPtr(T *d, RefCounts *c): data(d), counts(d ? c : 0) { incref(); }

public:
	WeakPtr(const WeakPtr &p): data(p.get()), counts(data ? p.counts : 0) { incref(); }

	template<typename U>
	WeakPtr(const WeakPtr<U> &p): data(p.get()), counts(data ? p.counts : 0) { incref(); }

	template<typename U>
	WeakPtr(const RefPtr<U> &p): data(p.data), counts(p.counts) { incref(); }

	WeakPtr &operator=(const WeakPtr &p) { return assign(p); }

	template<typename U>
	WeakPtr &operator=(const WeakPtr<U> &p) { return assign(p); }

	template<typename U>
	WeakPtr &operator=(const RefPtr<U> &p) { return assign(WeakPtr(p)); }

private:
	template<typename U>
	WeakPtr &assign(const WeakPtr<U> &);

	T *get() const { return (counts && counts->count ? data : 0); }
	void incref() { if(counts) ++counts->weak_count; }
	void decref();
};


template<typename T>
RefPtr<T> &RefPtr<T>::operator=(T *d)
{
	decref();
	data = d;
	counts = (d ? new RefCounts : 0);
	incref();
	return *this;
}

template<typename T>
template<typename U>
RefPtr<T> &RefPtr<T>::assign(const RefPtr<U> &p)
{
	decref();
	data = p.data;
	counts = p.counts;
	incref();
	return *this;
}

template<typename T>
T *RefPtr<T>::release()
{
	T *d = data;
	data = 0;
	decref();
	counts = 0;
	return d;
}

template<typename T>
void RefPtr<T>::decref()
{
	if(!counts) return;
	--counts->count;
	if(!counts->count)
	{
		delete data;
		data = 0;
	}
	else if(counts->count==RefCounts::KEEP)
		data = 0;
	else
		return;

	if(!counts->weak_count)
	{
		delete counts;
		counts = 0;
	}
}


template<typename T>
template<typename U>
WeakPtr<T> &WeakPtr<T>::assign(const WeakPtr<U> &p)
{
	decref();
	data = p.get();
	counts = (data ? p.counts : 0);
	incref();
	return *this;
}

template<typename T>
void WeakPtr<T>::decref()
{
	if(!counts) return;
	--counts->weak_count;
	if(!counts->weak_count && (!counts->count || counts->count==RefCounts::KEEP))
	{
		delete counts;
		data = 0;
		counts = 0;
	}
}

} // namespace Msp

#endif
