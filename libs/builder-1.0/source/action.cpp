/* $Id: action.cpp 89 2009-05-07 06:08:09Z tdb $

This file is part of builder
Copyright © 2006-2007, 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/io/print.h>
#include "action.h"

using namespace Msp;
using namespace std;

void Action::announce(const string &pkg, const string &tool, const string &tgt)
{
	IO::print("[%-10s] [%4s] %s\n", pkg.substr(0, 10), tool, tgt);
}
