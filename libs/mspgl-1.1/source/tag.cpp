/* $Id: tag.cpp 37 2007-12-27 17:12:27Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "tag.h"

namespace Msp {
namespace GL {

Tag::Tag(const char *s):
	id(0)
{
	for(const char *i=s; *i; ++i)
		id=id*id+*i;
}

Tag::Tag(const std::string &s):
	id(0)
{
	for(std::string::const_iterator i=s.begin(); i!=s.end(); ++i)
		id=id*id+*i;
}

} // namespace GL
} // namespace Msp
