/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/core/except.h>
#include "level.h"
#include "series.h"

using namespace std;
using namespace Msp;

const Level &Series::get_level(unsigned i) const
{
	if(i>=levels.size())
		throw InvalidParameterValue("Level index out of range");

	return *levels[i];
}


Series::Loader::Loader(Series &s, Collection &c):
	series(s),
	coll(c)
{
	add("level", &Loader::level);
}

void Series::Loader::level(const string &n)
{
	series.levels.push_back(coll.get<Level>(n));
}
