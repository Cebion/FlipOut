/* $Id: collection.h 45 2008-08-06 16:35:21Z tdb $

This file is part of libmspdatafile
Copyright © 2006-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_DATAFILE_COLLECTION_H_
#define MSP_DATAFILE_COLLECTION_H_

#include <msp/core/meta.h>
#include <msp/core/refptr.h>
#include "loader.h"

namespace Msp {
namespace DataFile {

/**
Helper struct to determine whether a Loader has a Collection typedef.
*/
template<typename T>
struct NeedsCollection
{
	struct Yes { char c[2]; };
	struct No { char c; };

	template<typename U>
	static Yes f(typename U::Collection *);
	template<typename U>
	static No f(...);

	enum { result=(sizeof(f<T>(0))==sizeof(Yes)) };
};

/**
A collection of objects that can be loaded from a datafile.  Each object is
identified by a name, which must be unique across the entire collection.
*/
class Collection
{
public:
	class Loader;

private:
	/* XXX I don't really like sticking all this stuff in here, but there's some
	   complex inter-class relationships, especially between ItemKeyword and
	   Collection::Loader. */

	struct ItemBase
	{
		virtual ~ItemBase() { }
	};

	template<typename T>
	struct Item: public ItemBase
	{
		T *data;

		Item(T *d): data(d) { }
		~Item() { delete data; }
	};

	/**
	Used to store keywords for types that can be loaded.
	*/
	struct ItemKeywordBase
	{
		virtual ~ItemKeywordBase() { }
		virtual void add_to_loader(Loader &) const { };
	};

	template<typename T, typename S, bool need_coll=NeedsCollection<typename T::Loader>::result>
	struct ItemKeyword: public ItemKeywordBase
	{
		std::string keyword;

		ItemKeyword(const std::string &kw): keyword(kw) { }

		void add_to_loader(Loader &ldr) const
		{ ldr.add(keyword, &Loader::item<T, S>); }
	};

	template<typename T, typename S>
	struct ItemKeyword<T, S, true>: public ItemKeywordBase
	{
		std::string keyword;

		ItemKeyword(const std::string &kw): keyword(kw) { }

		virtual void add_to_loader(Loader &ldr) const
		{ ldr.add(keyword, &Loader::coll_item<T, S, typename T::Loader::Collection>); }
	};

	/**
	Used to store types that can be created automatically.
	*/
	struct ItemCreatorBase
	{
		virtual ~ItemCreatorBase() { }

		template<typename S>
		bool create(Collection &coll, const std::string &name, S *&ptr)
		{
			ItemCreatorBridge<S> *creator=dynamic_cast<ItemCreatorBridge<S> *>(this);
			if(creator)
			{
				ptr=creator->create(coll, name);
				return true;
			}
			return false;
		}
	};

	template<typename S>
	struct ItemCreatorBridge: public ItemCreatorBase
	{
		virtual S *create(Collection &, const std::string &) const =0;
	};

	template<typename T, typename S, typename C>
	struct ItemCreator: public ItemCreatorBridge<S>
	{
		typedef T *(C::*fCreate)(const std::string &);

		fCreate create_func;

		ItemCreator(fCreate cf): create_func(cf) { }
		virtual S *create(Collection &coll, const std::string &name) const
		{ return (dynamic_cast<C &>(coll).*create_func)(name); }
	};

public:
	/**
	Loads objects into a Collection.
	*/
	class Loader: public DataFile::Loader
	{
	private:
		Collection &coll;

	public:
		Loader(Collection &);
		Collection &get_object() const { return coll; }
	private:
		template<typename T, typename S, typename C>
		void coll_item(const std::string &n)
		{
			RefPtr<T> it=new T;
			load_sub(*it, dynamic_cast<C &>(coll));
			coll.add<S>(n, it.get());
			it.release();
		}

		template<typename T, typename S>
		void item(const std::string &n)
		{
			RefPtr<T> it=new T;
			load_sub(*it);
			coll.add<S>(n, it.get());
			it.release();
		}

		template<typename, typename, bool> friend class ItemKeyword;
	};

private:
	typedef std::map<std::string, ItemBase *> ItemMap;
	typedef std::list<ItemKeywordBase *> ItemKeywordSeq;
	typedef std::list<ItemCreatorBase *> ItemCreatorSeq;

	ItemMap items;
	ItemKeywordSeq keywords;
	ItemCreatorSeq creators;

	Collection(const Collection &);
	Collection &operator=(const Collection &);
public:
	Collection() { }
	virtual ~Collection();

	/**
	Adds an object into the collection.  If a name collision occurs, an
	exception is thrown.  The collection takes ownership of the object.
	*/
	template<typename T>
	void add(const std::string &name, T *d)
	{
		if(items.count(name))
			throw KeyError("Duplicate key in collection", name);

		items[name]=new Item<typename RemoveConst<T>::Type>(d);
	}

	/**
	Gets an object of a specific type from the collection.
	*/
	template<typename T>
	T *get(const std::string &name) const
	{
		typedef typename RemoveConst<T>::Type NCT;

		ItemMap::const_iterator i=items.find(name);
		if(i==items.end())
			throw KeyError("Item not found in collection", name);

		const Item<NCT> *item=dynamic_cast<const Item<NCT> *>(i->second);
		if(!item)
			throw TypeError("Type mismatch on item '"+name+"'");

		return item->data;
	}

	/**
	Gets an object of a specific type from the collection.  If the name is not
	found in the collection and there is a creator for the item type, it is
	invoked.
	*/
	template<typename T>
	T *get(const std::string &name)
	{
		typedef typename RemoveConst<T>::Type NCT;

		ItemMap::const_iterator i=items.find(name);
		if(i==items.end())
		{
			for(ItemCreatorSeq::iterator j=creators.begin(); j!=creators.end(); ++j)
			{
				NCT *d=0;
				if((*j)->create(*this, name, d))
				{
					// We already know that the item didn't exist yet
					items[name]=new Item<NCT>(d);
					return d;
				}
			}
			throw KeyError("Item not found in collection", name);
		}

		const Item<NCT> *item=dynamic_cast<const Item<NCT> *>(i->second);
		if(!item)
			throw TypeError("Type mismatch on item '"+name+"'");

		return item->data;
	}

	/**
	Returns a list of the names of objects of a specific type in the collection.
	*/
	template<typename T>
	std::list<std::string> get_names() const
	{
		std::list<std::string> result;
		for(ItemMap::const_iterator i=items.begin(); i!=items.end(); ++i)
			if(dynamic_cast<const Item<typename RemoveConst<T>::Type> *>(i->second))
				result.push_back(i->first);
		return result;
	}

	/**
	Returns a list of objects of a specific type in the collection.
	*/
	template<typename T>
	std::list<T *> get_list() const
	{
		typedef typename RemoveConst<T>::Type NCT;

		std::list<T *> result;
		for(ItemMap::const_iterator i=items.begin(); i!=items.end(); ++i)
			if(Item<NCT> *item=dynamic_cast<Item<NCT> *>(i->second))
				result.push_back(item->data);
		return result;
	}

	/**
	Checks whether a name exists in the collection.  Does not care about the
	type of the object.
	*/
	bool contains(const std::string &n) const;

	/**
	Returns the name of an item in the collection.
	*/
	template<typename T>
	const std::string &get_name(T *d) const
	{
		typedef typename RemoveConst<T>::Type NCT;

		for(ItemMap::const_iterator i=items.begin(); i!=items.end(); ++i)
			if(Item<NCT> *item=dynamic_cast<Item<NCT> *>(i->second))
				if(item->data==d)
					return i->first;
	
		throw KeyError("Item not found in collection");
	}

protected:
	/**
	Adds a type that can be loaded from datafiles.
	*/
	template<typename T>
	void add_keyword(const std::string &keyword)
	{ add_keyword<T, T>(keyword); }

	/**
	Adds a type that can be loaded from datafiles, with different storage type.
	*/
	template<typename T, typename S>
	void add_keyword(const std::string &keyword)
	{ keywords.push_back(new ItemKeyword<T, S>(keyword)); }

	/**
	Adds a type that can be created automatically.
	*/
	template<typename T, typename C>
	void add_creator(T *(C::*func)(const std::string &))
	{ add_creator<T, T, C>(func); }

	template<typename T, typename S, typename C>
	void add_creator(T *(C::*func)(const std::string &))
	{ creators.push_back(new ItemCreator<T, S, C>(func)); }
};

} // namespace DataFile
} // namespace Msp

#endif
