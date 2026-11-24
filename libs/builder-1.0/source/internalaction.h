/* $Id: internalaction.h 84 2009-02-19 16:43:37Z tdb $

This file is part of builder
Copyright © 2007, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef INTERNALACTION_H_
#define INTERNALACTION_H_

#include <msp/core/thread.h>
#include "action.h"

class InternalAction: public Action
{
protected:
	class Worker: public Msp::Thread
	{
	public:
		bool get_done() const { return done; }
		bool get_error() const { return error; }
	protected:
		bool done;
		bool error;

		Worker();
	};

	Worker *worker;

	InternalAction(Builder &);
public:
	virtual ~InternalAction();

	virtual int check();
};

#endif
