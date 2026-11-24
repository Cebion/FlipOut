/* $Id: input.cpp 44 2008-07-23 13:02:32Z tdb $

This file is part of libmspdatafile
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#include "input.h"

namespace Msp {
namespace DataFile {

Input::Input(IO::Base &i):
	in(i),
	line(1),
	next(-1)
{ }

int Input::get()
{
	int c=next;
	next=-1;
	if(c<0) c=in.get();
	
	if(c=='\n')
		++line;

	return c;
}

int Input::peek()
{
	if(next<0)
		next=in.get();
	return next;
}

Input::operator bool() const
{
	return next>=0 || !in.eof();
}

} // namespace DataFile
} // namespace Msp
