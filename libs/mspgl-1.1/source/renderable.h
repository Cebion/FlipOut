/* $Id: renderable.h 63 2008-09-24 16:17:29Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_RENDERABLE_H_
#define MSP_GL_RENDERABLE_H_

#include <string>
#include "tag.h"

namespace Msp {
namespace GL {

class Renderable
{
protected:
	Renderable() { }
public:
	virtual ~Renderable() { }

	virtual bool has_pass(const Tag &tag) const =0;
	virtual void render(const Tag &tag=Tag()) const =0;
};

} // namespace Msp
} // namespace GL

#endif
