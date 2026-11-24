/* $Id: tar.h 84 2009-02-19 16:43:37Z tdb $

This file is part of builder
Copyright © 2007, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef TAR_H_
#define TAR_H_

#include <msp/core/thread.h>
#include "internalaction.h"
#include "misc.h"

class TarBall;

class Tar: public InternalAction
{
private:
	class Worker: public InternalAction::Worker
	{
	public:
		Worker(Tar &);
	private:
		Tar &tar;

		void main();
		void store_number(char *, unsigned, unsigned);
	};

	const TarBall &tarball;
	StringList files;

public:
	Tar(Builder &, const TarBall &);
};

#endif
