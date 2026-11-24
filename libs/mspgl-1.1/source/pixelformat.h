/* $Id: pixelformat.h 76 2009-01-27 19:38:22Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_PIXELFORMAT_H_
#define MSP_GL_PIXELFORMAT_H_

#include <istream>
#include <msp/gbase/pixelformat.h>
#include "gl.h"
#include <GL/glext.h>

namespace Msp {
namespace GL {

enum PixelFormat
{
	COLOR_INDEX     = GL_COLOR_INDEX,
	STENCIL_INDEX   = GL_STENCIL_INDEX,
	DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
	RED             = GL_RED,
	GREEN           = GL_GREEN,
	BLUE            = GL_BLUE,
	ALPHA           = GL_ALPHA,
	RGB             = GL_RGB,
	RGBA            = GL_RGBA,
	RGB8            = GL_RGB8,
	RGB16F          = GL_RGB16F_ARB,
	RGB32F          = GL_RGB32F_ARB,
	RGBA8           = GL_RGBA8,
	RGBA16F         = GL_RGBA16F_ARB,
	RGBA32F         = GL_RGBA32F_ARB,
	BGR             = GL_BGR,
	BGRA            = GL_BGRA,
	LUMINANCE       = GL_LUMINANCE,
	LUMINANCE8      = GL_LUMINANCE8,
	LUMINANCE16F    = GL_LUMINANCE16F_ARB,
	LUMINANCE32F    = GL_LUMINANCE32F_ARB,
	LUMINANCE_ALPHA    = GL_LUMINANCE_ALPHA,
	LUMINANCE_ALPHA8   = GL_LUMINANCE8_ALPHA8,
	LUMINANCE_ALPHA16F = GL_LUMINANCE_ALPHA16F_ARB,
	LUMINANCE_ALPHA32F = GL_LUMINANCE_ALPHA32F_ARB,
};

std::istream &operator>>(std::istream &, PixelFormat &);

PixelFormat pixelformat_from_graphics(Graphics::PixelFormat);

} // namespace GL
} // namespace Msp

#endif
