/* $Id: copy.h 84 2009-02-19 16:43:37Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef COPY_H_
#define COPY_H_

#include <msp/core/thread.h>
#include <msp/fs/path.h>
#include "internalaction.h"

class Package;

/**
Copies a file to another place.  Used by the Install target.
*/
class Copy: public InternalAction
{
private:
	/**
	A worker thread that actually does the data transfer.
	*/
	class Worker: public InternalAction::Worker
	{
	public:
		Worker(Copy &);
	private:
		Copy &copy;

		void main();
	};

	Msp::FS::Path src;
	Msp::FS::Path dest;

public:
	Copy(Builder &, const Package &, const Msp::FS::Path &, const Msp::FS::Path &);
};

#endif
