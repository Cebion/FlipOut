/* $Id: objectinstance.cpp 34 2007-11-04 22:11:27Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "object.h"
#include "objectinstance.h"
#include "programdata.h"

using namespace std;

namespace Msp {
namespace GL {

ObjectInstance::ObjectInstance(const Object &obj):
	object(obj)
{ }

bool ObjectInstance::has_pass(const Tag &tag) const
{
	return object.has_pass(tag);
}

void ObjectInstance::render(const Tag &tag) const
{
	object.render(*this, tag);
}

} // namespace GL
} // namespaec Msp
