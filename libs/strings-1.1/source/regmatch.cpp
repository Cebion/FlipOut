/* $Id$

This file is part of libmspstrings
Copyright © 2007 Mikko Rasa
Distributed under the LGPL
*/
#include <msp/core/except.h>
#include "regmatch.h"

using namespace std;

namespace Msp {

RegMatch::RegMatch(const string &str, const GroupArray &g):
	groups(g)
{
	for(GroupArray::iterator i=groups.begin(); i!=groups.end(); ++i)
		if(i->match)
		{
			i->length=i->end-i->begin;
			i->str=str.substr(i->begin, i->length);
		}
}

const RegMatch::Group &RegMatch::group(unsigned i) const
{
	if(i>=groups.size())
		throw InvalidParameterValue("Group index out of range");
	return groups[i];
}

} // namespace Msp
