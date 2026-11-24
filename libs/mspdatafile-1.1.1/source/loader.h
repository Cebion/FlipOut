/* $Id: loader.h 51 2008-09-12 16:07:50Z tdb $

This file is part of libmspdatafile
Copyright © 2006-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_DATAFILE_LOADER_H_
#define MSP_DATAFILE_LOADER_H_

#include <map>
#include <msp/io/buffered.h>
#include <msp/io/file.h>
#include "except.h"
#include "loaderaction.h"
#include "parser.h"
#include "statement.h"

namespace Msp {
namespace DataFile {

/**
Base class for data loaders.  This class only provides core functionality.
You'll almost certainly want to use one of the BasicLoader classes instead.

Under normal circumstances, a class capable of being loaded should have a
nested typed called Loader which resolves to a descendant of this class.  If
another structure is used, the loader object must be constructed manually.

A loader class should execute one or more calls to the various add() functions
to register actions with expected keywords.  Currently possible actions are
calling a function of the loader, storing values in member variables of an
object and ignoring the statement.  If a unexpected keyword is encountered, an
exception is thrown and the loading is aborted.

A sub-object can be loaded with one of the load_sub functions.

When loading has finished successfully, the virtual function finish() is
called.  Any post-processing of the data should be placed here and not in the
destructor.

See also classes BasicLoader and BasicLoader2.
*/
class Loader
{
private:
	/**
	Loads data from a statement.
	*/
	void load(const Statement &st);

public:
	/**
	Loads statements from a parser.
	*/
	void load(Parser &p);

	virtual ~Loader();
protected:
	Loader(): cur_st(0) { }

	/**
	Adds a keyword that is loaded with a zero-argument function.
	*/
	template<typename L>
	void add(const std::string &k, void (L::*func)())
	{ add(k, new LoaderFunc0<L>(func)); }

	template<typename L, typename A0>
	void add(const std::string &k, void (L::*func)(A0))
	{ add(k, new LoaderFunc1<L, A0>(func)); }

	template<typename L, typename A0, typename A1>
	void add(const std::string &k, void (L::*func)(A0, A1))
	{ add(k, new LoaderFunc2<L, A0, A1>(func)); }

	template<typename L, typename A0, typename A1, typename A2>
	void add(const std::string &k, void (L::*func)(A0, A1, A2))
	{ add(k, new LoaderFunc3<L, A0, A1, A2>(func)); }

	template<typename L, typename A0, typename A1, typename A2, typename A3>
	void add(const std::string &k, void (L::*func)(A0, A1, A2, A3))
	{ add(k, new LoaderFunc4<L, A0, A1, A2, A3>(func)); }

	template<typename L, typename A0, typename A1, typename A2, typename A3, typename A4>
	void add(const std::string &k, void (L::*func)(A0, A1, A2, A3, A4))
	{ add(k, new LoaderFunc5<L, A0, A1, A2, A3, A4>(func)); }

	/**
	Adds a keyword that is loaded into a variable of the loaded object.
	*/
	template<typename L, typename T0>
	void add(const std::string &k, T0 L::*p0)
	{ add(k, new LoadValue1<L, T0>(p0)); }

	template<typename L, typename T0, typename T1>
	void add(const std::string &k, T0 L::*p0, T1 L::*p1)
	{ add(k, new LoadValue2<L, T0, T1>(p0, p1)); }

	/**
	Adds a keyword that is recognized but ignored.
	*/
	void add(const std::string &k)
	{ add(k, 0); }

	/**
	Loads a sub-object from the statement being processed.  The Loader class of
	the sub-object is automatically used.
	*/
	template<typename S>
	void load_sub(S &s)
	{
		typename S::Loader ldr(s);
		load_sub_with(ldr);
	}

	/**
	Loads a sub-object from the statement being processed with an extra parameter
	for the Loader.  The Loader class of the sub-object is automatically used.
	*/
	template<typename S, typename T>
	void load_sub(S &s, T &p)
	{
		typename S::Loader ldr(s, p);
		load_sub_with(ldr);
	}

	/**
	Processes the current statement's substatements with another Loader.
	*/
	void load_sub_with(Loader &);

	/**
	Returns the source of the statement being processed.  This can be used to
	implement relative paths in include-like statements.  Note that the source
	may not necessarily be a file.
	*/
	const std::string &get_source() const
	{
		if(!cur_st)
			throw InvalidState("get_source called without current statement");
		return cur_st->source;
	}

	virtual void finish() { }
private:
	typedef std::map<std::string, LoaderAction *> ActionMap;

	ActionMap       actions;
	const Statement *cur_st;

	void add(const std::string &, LoaderAction *);
	void load_statement(const Statement &st);
};


/**
Provides the basic functionality of an object loader.  Deriving from this
allows loading values directly into member variables of the objects.
*/
template<typename O>
class BasicLoader: public Loader
{
public:
	typedef O Object;

protected:
	O &obj;

public:
	BasicLoader(O &o): obj(o) { }
	O &get_object() const { return obj; }
};


/**
Provides functionality for loading objects with a Collection.  Deriving from
this allows loading pointers to objects in the collection automatically.
*/
template<typename O, typename C>
class BasicLoader2: public BasicLoader<O>
{
public:
	typedef C Collection;

protected:
	C &coll;

public:
	BasicLoader2(O &o, C &c): BasicLoader<O>(o), coll(c) { }
	C &get_collection() const { return coll; }
};


/**
Loads an object from a file.  The object must have a public Loader class.
*/
template<typename T>
void load(T &obj, const std::string &fn)
{
	IO::File in(fn);
	IO::Buffered buf(in);

	Parser parser(buf, fn);
	typename T::Loader loader(obj);
	loader.load(parser);
}

template<typename T, typename U>
void load(T &obj, const std::string &fn, U &arg)
{
	IO::File in(fn);
	IO::Buffered buf(in);

	Parser parser(buf, fn);
	typename T::Loader loader(obj, arg);
	loader.load(parser);
}

} // namespace DataFile
} // namespace Msp

#endif
