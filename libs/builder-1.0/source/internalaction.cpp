/* $Id: internalaction.cpp 84 2009-02-19 16:43:37Z tdb $

This file is part of builder
Copyright © 2007, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "internalaction.h"

InternalAction::InternalAction(Builder &b):
	Action(b),
	worker(0)
{ }

InternalAction::~InternalAction()
{
	delete worker;
}

int InternalAction::check()
{
	if(!worker)  // True for dry run
	{
		signal_done.emit();
		return 0;
	}

	if(worker->get_done())
	{
		signal_done.emit();
		worker->join();
		return worker->get_error()?1:0;
	}

	return -1;
}


InternalAction::Worker::Worker():
	done(false),
	error(false)
{ }
