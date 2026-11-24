/* $Id: except.cpp 60 2008-11-13 10:40:34Z tdb $

This file is part of libmspcore
Copyright © 2006-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <sstream>
#include <cstring>
#ifdef WIN32
#include <windows.h>
#endif
#include "except.h"

using namespace std;

namespace Msp {

Exception::Exception(const string &w):
	wot(w)
{
#ifdef WITH_EXCEPTION_BACKTRACE
	bt=Debug::Backtrace::create();
#endif
}

Exception &Exception::at(const std::string &w) throw()
{
	wer=w;
	wot=wer+": "+wot;
	return *this;
}


SystemError::SystemError(const string &w_, int e):
	Exception(build_what(w_, e)),
	err(e)
{ }

string SystemError::build_what(const string &w, int e)
{
	ostringstream buf;
	buf<<w<<": ";
#ifdef WIN32
	char msg[1024];
	if(FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, e, 0, msg, sizeof(msg), 0))
		buf<<msg;
	else
		buf<<e;
#else
	buf<<strerror(e);
#endif
	return buf.str();
}


KeyError::KeyError(const string &w_, const string &k):
	Exception(w_+" ("+k+")"),
	key(k)
{ }

} // namespace Msp
