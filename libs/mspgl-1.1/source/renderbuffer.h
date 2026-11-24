/* $Id: renderbuffer.h 84 2009-09-21 13:50:58Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_RENDERBUFFER_H_
#define MSP_GL_RENDERBUFFER_H_

#include "pixelformat.h"
#include "types.h"

namespace Msp {
namespace GL {

/**
A Renderbuffer contains a single renderable image.  It can be attached to a
Framebuffer to provide a logical buffer that is required to render the scene
correctly but that is not needed as a texture later.

Requires the GL_EXT_framebuffer_object extension.
*/
class Renderbuffer
{
private:
	uint id;
	sizei width;
	sizei height;

public:
	Renderbuffer();
	~Renderbuffer();

	uint get_id() const { return id; }
	sizei get_width() const { return width; }
	sizei get_height() const { return height; }

	void bind() const;

	void storage(PixelFormat fmt, sizei width, sizei height);
};

} // namespace GL
} // namespace Msp

#endif
