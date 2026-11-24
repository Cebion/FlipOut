/* $Id: collection.cpp 29 2007-10-15 14:16:41Z tdb $

This file is part of libmspdatafile
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "collection.h"

namespace Msp {
namespace DataFile {

Collection::~Collection()
{
	for(ItemMap::iterator i=items.begin(); i!=items.end(); ++i)
		delete i->second;
	for(ItemKeywordSeq::iterator i=keywords.begin(); i!=keywords.end(); ++i)
		delete *i;
	for(ItemCreatorSeq::iterator i=creators.begin(); i!=creators.end(); ++i)
		delete *i;
}

bool Collection::contains(const std::string &n) const
{
	return items.count(n);
}


Collection::Loader::Loader(Collection &c):
	coll(c)
{	
	for(ItemKeywordSeq::const_iterator i=coll.keywords.begin(); i!=coll.keywords.end(); ++i)
		(*i)->add_to_loader(*this);
}

} // namespace DataFile
} // namespace Msp
