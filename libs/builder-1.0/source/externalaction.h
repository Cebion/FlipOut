/* $Id: externalaction.h 84 2009-02-19 16:43:37Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef EXTERNALACTION_H_
#define EXTERNALACTION_H_

#include <list>
#include <string>
#include "action.h"
#include "misc.h"

/**
Base class for Actions that want to execute an external program.
*/
class ExternalAction: public Action
{
protected:
	StringList argv;
	Msp::FS::Path work_dir;
	int pid;
	int exit_code;
	
	ExternalAction(Builder &b): Action(b), pid(0), exit_code(0) { }

	/** Starts the external program.  Fill in argv before calling this. */
	void launch();

public:
	virtual int check();
};

#endif
