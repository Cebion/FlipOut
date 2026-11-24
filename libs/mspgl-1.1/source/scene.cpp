/* $Id: scene.cpp 59 2008-09-03 06:40:23Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "object.h"
#include "objectinstance.h"
#include "scene.h"

using namespace std;

namespace Msp {
namespace GL {

void Scene::add(const Renderable &r)
{
	if(const ObjectInstance *oi=dynamic_cast<const ObjectInstance *>(&r))
		objects[&oi->get_object()].insert(oi);
	else
		renderables.insert(&r);
}

void Scene::remove(const Renderable &r)
{
	if(const ObjectInstance *oi=dynamic_cast<const ObjectInstance *>(&r))
	{
		map<const Object *, set<const ObjectInstance *> >::iterator i=objects.find(&oi->get_object());
		if(i!=objects.end())
		{
			i->second.erase(oi);
			if(i->second.empty())
				objects.erase(i);
		}
	}
	else
		renderables.erase(&r);
}

void Scene::render(const Tag &tag) const
{
	for(map<const Object *, set<const ObjectInstance *> >::const_iterator i=objects.begin(); i!=objects.end(); ++i)
		if(i->first->has_pass(tag))
			i->first->render(i->second.begin(), i->second.end(), tag);

	for(set<const Renderable *>::const_iterator i=renderables.begin(); i!=renderables.end(); ++i)
		if((*i)->has_pass(tag))
			(*i)->render(tag);
}

} // namespace GL
} // namespace Msp
