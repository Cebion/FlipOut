/* $Id: rendermode.h 38 2007-12-29 20:47:08Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_RENDERMODE_H_
#define MSP_GL_RENDERMODE_H_

#include "gl.h"

namespace Msp {
namespace GL {

enum RenderMode
{
	RENDER   = GL_RENDER,
	SELECT   = GL_SELECT,
	FEEDBACK = GL_FEEDBACK
};

int render_mode(RenderMode);

} // namespace GL
} // namespace Msp

#endif
