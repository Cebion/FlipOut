/* $Id: demangle.cpp 60 2008-11-13 10:40:34Z tdb $

This file is part of libmspcore
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cstdlib>
#ifdef __GNUC__
#include <cxxabi.h>
#endif
#include "demangle.h"

using namespace std;

namespace Msp {
namespace Debug {

string demangle(const string &sym)
{
#ifdef __GNUC__
	int status;
	char *dm=abi::__cxa_demangle(sym.c_str(), 0, 0, &status);
	
	string result;
	if(status==0)
		result=dm;
	else
		result=sym;
	
	free(dm);

	return result;
#else
	return sym;
#endif
}

} // namespace Debug
} // namespace Msp
