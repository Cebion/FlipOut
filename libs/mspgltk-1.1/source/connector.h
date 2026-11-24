/* $Id: connector.h 21 2007-12-26 09:20:37Z tdb $

This file is part of libmspgltk
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GLTK_CONNECTOR_H_
#define MSP_GLTK_CONNECTOR_H_

#include <map>
#include <string>
#include <sigc++/functors/mem_fun.h>

namespace Msp {
namespace GLtk {

class Connector;
class Logic;
class Widget;

class ConnAction
{
public:
	virtual void connect(Connector &conn, Widget &wdg, const std::string &data) const =0;
	virtual ~ConnAction() { }
};

template<typename C, typename W>
class ConnFunc0: public ConnAction
{
public:
	typedef void (C::*FuncType)(W &);

	ConnFunc0(FuncType f): func(f) { }
	virtual void connect(Connector &conn, Widget &wdg, const std::string &) const
	{
		(dynamic_cast<C &>(conn).*func)(dynamic_cast<W &>(wdg));
	}

private:
	FuncType func;
};

template<typename C, typename W>
class ConnFunc1: public ConnAction
{
public:
	typedef void (C::*FuncType)(W &, const std::string &);

	ConnFunc1(FuncType f): func(f) { }
	virtual void connect(Connector &conn, Widget &wdg, const std::string &data) const
	{
		(dynamic_cast<C &>(conn).*func)(dynamic_cast<W &>(wdg), data);
	}

private:
	FuncType func;
};

template<typename C, typename W, typename S, typename F>
class ConnSignal: public ConnAction
{
public:
	ConnSignal(S W::*s, F f): signal(s), func(f) { }
	virtual void connect(Connector &conn, Widget &wdg, const std::string &) const
	{
		(dynamic_cast<W &>(wdg).*signal).connect(sigc::mem_fun(&dynamic_cast<C &>(conn).get_object(), func));
	}

private:
	S W::*signal;
	F func;
};

/**
Provides an interface for associating the bindings stored in a Logic object
with actual code.  Derive a class from this and use the add functions to
specify handlers for each binding type.

Bindings are normally handled by member functions of the Connector class.  The
function must take a reference to a widget (of any type) as its first parameter.
If it takes a second parameter, the binding data is passed in as well.

As a shortcut for simple connections, signals of widgets can be connected
directly to a handler object.  For this to work, the Connector class must be a
public inner class of the handler class and it must have a get_object() member
function returning a reference to the handler object.

TODO: lexical_cast the binding data (requires working around references)
*/
class Connector
{
private:
	std::map<std::string, ConnAction *> actions;

protected:
	Connector() { }
public:
	virtual ~Connector();

	/**
	Processes all bindings in the Logic object and calls appropriate handlers.
	*/
	void connect(const Logic &);

protected:
	/**
	Adds a handler function for a binding.
	*/
	template<typename C, typename W>
	void add(const std::string &type, void (C::*func)(W &))
	{ add(type, new ConnFunc0<C, W>(func)); }

	/**
	Adds a handler function for a binding.  The binding data is passed in the
	second parameter.
	*/
	template<typename C, typename W>
	void add(const std::string &type, void (C::*func)(W &, const std::string &))
	{ add(type, new ConnFunc1<C, W>(func)); }

	/**
	Adds a signal connector for a binding.
	*/
	template<typename W, typename S, typename H, typename F>
	void add(const std::string &type, S W::*signal, F H::*func)
	{ add(type, new ConnSignal<typename H::Connector, W, S, F H::*>(signal, func)); }

private:
	void add(const std::string &, ConnAction *);
};

} // namespace GLtk
} // namespace Msp

#endif
