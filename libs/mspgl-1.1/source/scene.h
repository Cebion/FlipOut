/* $Id: scene.h 59 2008-09-03 06:40:23Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_SCENE_H_
#define MSP_GL_SCENE_H_

#include <map>
#include <set>
#include "renderable.h"

namespace Msp {
namespace GL {

class Object;
class ObjectInstance;

class Scene: public Renderable
{
private:
	std::map<const Object *, std::set<const ObjectInstance *> > objects;
	std::set<const Renderable *> renderables;

public:
	void add(const Renderable &);
	void remove(const Renderable &);

	virtual bool has_pass(const Tag &) const { return true; }
	virtual void render(const Tag &tag=Tag()) const;
};

} // namespace GL
} // namespace Msp

#endif
