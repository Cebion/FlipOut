/* $Id: texenv.h 62 2008-09-24 16:10:43Z tdb $

This file is part of libmspgl
Copyright © 2007-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_TEXENV_H_
#define MSP_GL_TEXENV_H_

#include "color.h"
#include "gl.h"

namespace Msp {
namespace GL {

enum TexEnvMode
{
	REPLACE  = GL_REPLACE,
	MODULATE = GL_MODULATE,
	DECAL    = GL_DECAL,
	BLEND    = GL_BLEND,
	ADD      = GL_ADD,
	COMBINE  = GL_COMBINE
};

class TexEnv
{
private:
	TexEnvMode mode;
	Color color;

public:
	TexEnv();
	void set_mode(TexEnvMode);
	void set_color(const Color &);
	TexEnvMode get_mode() const { return mode; }
	const Color &get_color() const { return color; }
	void bind();

	static void unbind();
};

} // namespace GL
} // namespace Msp

#endif
