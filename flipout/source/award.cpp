/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "award.h"

unsigned Award::get_score(unsigned level) const
{
	std::map<unsigned, unsigned>::const_iterator i=levels.upper_bound(level);
	if(i==levels.begin())
		return 0;
	
	--i;
	return i->second;
}


Award::Loader::Loader(Award &a):
	award(a)
{
	add("level", &Loader::level);
}

void Award::Loader::level(unsigned l, unsigned s)
{
	award.levels[l]=s;
}
