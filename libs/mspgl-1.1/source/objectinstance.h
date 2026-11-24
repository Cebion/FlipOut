/* $Id: objectinstance.h 66 2008-10-04 20:26:55Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_OBJETCINSTANCE_H_
#define MSP_GL_OBJETCINSTANCE_H_

#include <string>
#include "renderable.h"

namespace Msp {
namespace GL {

class Object;
class ProgramData;

/**
Represents a single instance of an Object.  An application can derive another
class from this and overload the hook functions to specify location and other
instance-specific parameters for the rendered objects.

The finish_render function generally should clean up everything done by
setup_render, unless the rendering sequence is known to allow otherwise.
*/
class ObjectInstance: public Renderable
{
protected:
	const Object &object;

public:
	ObjectInstance(const Object &);

	const Object &get_object() const { return object; }

	virtual bool has_pass(const Tag &tag) const;

	virtual void render(const Tag &tag=Tag()) const;

	/**
	Hook function, called from Object just before rendering the mesh.
	*/
	virtual void setup_render(const Tag &) const { }

	/**
	Hook function, called from Object right after rendering the mesh.
	*/
	virtual void finish_render(const Tag &) const { }

	virtual unsigned get_level_of_detail() const { return 0; }
};

} // namespace GL
} // namespaec Msp

#endif
