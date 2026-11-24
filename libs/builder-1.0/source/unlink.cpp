/* $Id: unlink.cpp 87 2009-03-17 11:18:42Z tdb $

This file is part of builder
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/fs/utils.h>
#include "filetarget.h"
#include "sourcepackage.h"
#include "unlink.h"

Unlink::Unlink(Builder &b, const FileTarget &t):
	Action(b)
{
	const SourcePackage &spkg=*static_cast<const SourcePackage *>(t.get_package());

	announce(spkg.get_name(), "RM", relative(t.get_path(), spkg.get_source()).str());

	try
	{
		unlink(t.get_path());
	}
	catch(const Msp::SystemError &)
	{ }
}

int Unlink::check()
{
	signal_done.emit();
	return 0;
}
