/* $Id: texture3d.cpp 61 2008-09-08 19:39:39Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cmath>
#include <msp/gbase/image.h>
#include "except.h"
#include "extension.h"
#include "texture3d.h"
#include "version_1_2.h"

using namespace std;

namespace Msp {
namespace GL {

Texture3D::Texture3D():
	width(0),
	height(0),
	depth(0)
{
	static RequireVersion _ver(1, 3);

	target=GL_TEXTURE_3D;
	bind();
}

void Texture3D::storage(PixelFormat f, sizei w, sizei h, sizei d, int b)
{
	if(width>0)
		throw InvalidState("Textures may only be created once");
	if(w==0 || h==0 || d==0)
		throw InvalidParameterValue("Invalid texture dimensions");

	width=w;
	height=h;
	depth=d;
	ifmt=f;
	border=b;

	image(0, ifmt, UNSIGNED_BYTE, 0);
}

void Texture3D::image(int level, PixelFormat fmt, DataType type, const void *data)
{
	maybe_bind();
	glTexImage3D(target, level, ifmt, width, height, depth, border, fmt, type, data);
}

void Texture3D::load_image(const string &fn, int dp)
{
	Graphics::Image img;
	img.load_file(fn);

	unsigned w=img.get_width();
	unsigned h=img.get_height();
	unsigned d=1;

	if(dp==-1)
	{
		if(h%w)
			throw IncompatibleData("Image height is not divisible by its width");
		d=h/w;
		h=w;
	}
	else if(dp==-2)
	{
		for(d=h; d*d>h; d>>=2) ;
		for(; d*d<h; ++d) ;
		if(d*d!=h)
			throw IncompatibleData("Could not find a square root of texture height");
		h=d;
	}
	else if(dp>0)
		d=dp;

	PixelFormat fmt=pixelformat_from_graphics(img.get_format());
	if(width==0)
		storage(fmt, w, h, d, 0);
	else if(w!=width || h!=height || d!=depth)
		throw IncompatibleData("Image does not match texture storage");

	image(0, fmt, UNSIGNED_INT, img.get_data());
}

} // namespace GL
} // namespace Msp
