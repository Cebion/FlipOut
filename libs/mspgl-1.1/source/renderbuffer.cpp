/* $Id: renderbuffer.cpp 84 2009-09-21 13:50:58Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "extension.h"
#include "ext_framebuffer_object.h"
#include "renderbuffer.h"

namespace Msp {
namespace GL {

Renderbuffer::Renderbuffer()
{
	static RequireExtension _ext("GL_EXT_framebuffer_object");

	glGenRenderbuffersEXT(1, &id);
	bind();
}

Renderbuffer::~Renderbuffer()
{
	glDeleteRenderbuffersEXT(1, &id);
}

void Renderbuffer::bind() const
{
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, id);
}

void Renderbuffer::storage(PixelFormat fmt, sizei w, sizei h)
{
	bind();
	width=w;
	height=h;
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, fmt, width, height);
}

} // namespace GL
} // namespace Msp
