/* $Id: loaderaction.h 51 2008-09-12 16:07:50Z tdb $

This file is part of libmspdatafile
Copyright © 2006-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_DATAFILE_LOADERACTION_H_
#define MSP_DATAFILE_LOADERACTION_H_

#include "except.h"
#include "statement.h"

namespace Msp {
namespace DataFile {

class Loader;

/**
Base class for loader actions.
*/
class LoaderAction
{
public:
	/**
	Called when a statement is to be loaded.
	*/
	virtual void execute(Loader &, const Statement &) const=0;
	virtual ~LoaderAction() { }
protected:
	LoaderAction() { }
};


/**
Loads a statement by calling a function that takes no arguments.
*/
template<typename L>
class LoaderFunc0: public LoaderAction
{
public:
	typedef void (L::*FuncType)();

	LoaderFunc0(FuncType f): func(f) { }
	void execute(Loader &l, const Statement &st) const
	{
		if(st.args.size()!=0) throw TypeError("Wrong number of arguments");
		(dynamic_cast<L &>(l).*func)();
	};
private:
	FuncType func;
};


/**
Loads a statement by calling a function that takes one argument.
*/
template<typename L, typename A0>
class LoaderFunc1: public LoaderAction
{
public:
	typedef void (L::*FuncType)(A0);

	LoaderFunc1(FuncType f): func(f) { }
	void execute(Loader &l, const Statement &st) const
	{
		if(st.args.size()!=1) throw TypeError("Wrong number of arguments");
		(dynamic_cast<L &>(l).*func)(st.args[0].get<A0>());
	}
private:
	FuncType func;
};


/**
Loads a statement by calling a function that takes an array of values.
*/
template<typename L, typename A0>
class LoaderFunc1<L, const std::vector<A0> &>: public LoaderAction
{
public:
	typedef void (L::*FuncType)(const std::vector<A0> &);

	LoaderFunc1(FuncType f): func(f) { }
	void execute(Loader &l, const Statement &st) const
	{
		std::vector<A0> values;
		values.reserve(st.args.size());
		for(ValueArray::const_iterator i=st.args.begin(); i!=st.args.end(); ++i)
			values.push_back(i->get<A0>());
		(dynamic_cast<L &>(l).*func)(values);
	}
private:
	FuncType func;
};


/**
Loads a statement by calling a function with the statement itself as argument.
*/
template<typename L>
class LoaderFunc1<L, const Statement &>: public LoaderAction
{
public:
	typedef void (L::*FuncType)(const Statement &);

	LoaderFunc1(FuncType f): func(f) { }
	void execute(Loader &l, const Statement &st) const
	{
		(dynamic_cast<L &>(l).*func)(st);
	}
private:
	FuncType func;
};


template<typename L, typename A0, typename A1>
class LoaderFunc2: public LoaderAction
{
public:
	typedef void (L::*FuncType)(A0, A1);

	LoaderFunc2(FuncType f): func(f) { }
	void execute(Loader &l, const Statement &st) const
	{
		if(st.args.size()!=2) throw TypeError("Wrong number of arguments");
		(dynamic_cast<L &>(l).*func)(st.args[0].get<A0>(), st.args[1].get<A1>());
	}
private:
	FuncType func;
};


template<typename L, typename A0, typename A1, typename A2>
class LoaderFunc3: public LoaderAction
{
public:
	typedef void (L::*FuncType)(A0, A1, A2);

	LoaderFunc3(FuncType f): func(f) { }
	void execute(Loader &l, const Statement &st) const
	{
		if(st.args.size()!=3) throw TypeError("Wrong number of arguments");
		(dynamic_cast<L &>(l).*func)(st.args[0].get<A0>(), st.args[1].get<A1>(), st.args[2].get<A2>());
	}
private:
	FuncType func;
};


template<typename L, typename A0, typename A1, typename A2, typename A3>
class LoaderFunc4: public LoaderAction
{
public:
	typedef void (L::*FuncType)(A0, A1, A2, A3);

	LoaderFunc4(FuncType f): func(f) { }
	void execute(Loader &l, const Statement &st) const
	{
		if(st.args.size()!=4) throw TypeError("Wrong number of arguments");
		(dynamic_cast<L &>(l).*func)(st.args[0].get<A0>(), st.args[1].get<A1>(), st.args[2].get<A2>(), st.args[3].get<A3>());
	}
private:
	FuncType func;
};


template<typename L, typename A0, typename A1, typename A2, typename A3, typename A4>
class LoaderFunc5: public LoaderAction
{
public:
	typedef void (L::*FuncType)(A0, A1, A2, A3, A4);

	LoaderFunc5(FuncType f): func(f) { }
	void execute(Loader &l, const Statement &st) const
	{
		if(st.args.size()!=5) throw TypeError("Wrong number of arguments");
		(dynamic_cast<L &>(l).*func)(st.args[0].get<A0>(), st.args[1].get<A1>(), st.args[2].get<A2>(), st.args[3].get<A3>(), st.args[4].get<A4>());
	}
private:
	FuncType func;
};


template<typename L, typename T0>
class LoadValue1: public LoaderAction
{
public:
	typedef T0 L::*Pointer0Type;

	LoadValue1(Pointer0Type p0): ptr0(p0) { }
	void execute(Loader &l, const Statement &st) const
	{
		if(st.args.size()!=1) throw TypeError("Wrong number of arguments");
		dynamic_cast<typename L::Loader &>(l).get_object().*ptr0=st.args[0].get<T0>();
	}
private:
	Pointer0Type ptr0;
};


template<typename L, typename T0>
class LoadValue1<L, T0 *>: public LoaderAction
{
public:
	typedef T0 *L::*Pointer0Type;

	LoadValue1(Pointer0Type p0): ptr0(p0) { }
	void execute(Loader &l, const Statement &st) const
	{
		if(st.args.size()!=1) throw TypeError("Wrong number of arguments");
		typename L::Loader &ldr=dynamic_cast<typename L::Loader &>(l);
		ldr.get_object().*ptr0=ldr.get_collection().template get<T0>(st.args[0].get<std::string>());
	}
private:
	Pointer0Type ptr0;
};


template<typename L, typename T0, typename T1>
class LoadValue2: public LoaderAction
{
public:
	typedef T0 L::*Pointer0Type;
	typedef T1 L::*Pointer1Type;

	LoadValue2(Pointer0Type p0, Pointer1Type p1): ptr0(p0), ptr1(p1) { }
	void execute(Loader &l, const Statement &st) const
	{
		if(st.args.size()!=2) throw TypeError("Wrong number of arguments");
		dynamic_cast<typename L::Loader &>(l).get_object().*ptr0=st.args[0].get<T0>();
		dynamic_cast<typename L::Loader &>(l).get_object().*ptr1=st.args[1].get<T1>();
	}
private:
	Pointer0Type ptr0;
	Pointer1Type ptr1;
};

} // namespace DataFile
} // namespace Msp

#endif
