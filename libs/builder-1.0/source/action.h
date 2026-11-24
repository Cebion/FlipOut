/* $Id: action.h 84 2009-02-19 16:43:37Z tdb $

This file is part of builder
Copyright © 2006-2007, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <sigc++/sigc++.h>

class Builder;

/**
Actions are executed to rebuild targets.
*/
class Action
{
public:
	/// Emitted when the action has finished
	sigc::signal<void> signal_done;
	
protected:
	Builder &builder;
	
	Action(Builder &b): builder(b) { }
public:
	virtual ~Action() { }

	/**
	Checks whether the action is done and emits signal_done if it is.  Returns 0
	if the action has completed successfully, 1 if an error was encountered and
	-1 if it is still executing.
	*/
	virtual int check()=0;
	
protected:
	/**
	Annouces the action by printing out the package name, tool and target name.
	*/
	void announce(const std::string &, const std::string &, const std::string &);
};

#endif
