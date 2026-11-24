/* $Id: texture2d.cpp 55 2008-08-17 22:16:15Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "except.h"
#include "texture2d.h"

using namespace std;

namespace Msp {
namespace GL {

Texture2D::Texture2D():
	width(0),
	height(0)
{
	target=GL_TEXTURE_2D;
	bind();
}

void Texture2D::storage(PixelFormat fmt, sizei wd, sizei ht, int brd)
{
	if(width>0)
		throw InvalidState("Texture storage may only be specified once");
	if(wd==0 || ht==0)
		throw InvalidParameterValue("Invalid texture dimensions");

	ifmt=fmt;
	width=wd;
	height=ht;
	border=brd;
}

void Texture2D::image(int level, PixelFormat fmt, DataType type, const void *data)
{
	if(width==0)
		throw InvalidState("Texture storage has not been specified");

	maybe_bind();

	glTexImage2D(target, level, ifmt, width, height, border, fmt, type, data);
}

void Texture2D::sub_image(int level, int x, int y, sizei wd, sizei ht, PixelFormat fmt, DataType type, const void *data)
{
	if(width==0)
		throw InvalidState("Texture storage has not been specified");

	maybe_bind();

	glTexSubImage2D(target, level, x, y, wd, ht, fmt, type, data);
}

void Texture2D::load_image(const string &fn)
{
	Graphics::Image img;
	img.load_file(fn);

	image(img);
}

void Texture2D::image(const Graphics::Image &img)
{
	unsigned w=img.get_width();
	unsigned h=img.get_height();
	PixelFormat fmt=pixelformat_from_graphics(img.get_format());
	if(width==0)
		storage(fmt, w, h, 0);
	else if(w!=width || h!=height)
		throw IncompatibleData("Image does not match texture storage");

	image(0, fmt, UNSIGNED_BYTE, img.get_data());
}


Texture2D::Loader::Loader(Texture2D &t):
	Texture::Loader(t)
{
	add("image_data", &Loader::image_data);
	add("raw_data", &Loader::raw_data);
	add("storage", &Loader::storage);
}

void Texture2D::Loader::image_data(const string &data)
{
	Graphics::Image img;
	img.load_memory(data.data(), data.size());

	static_cast<Texture2D &>(tex).image(img);
}

void Texture2D::Loader::raw_data(const string &data)
{
	Texture2D &t2d=static_cast<Texture2D &>(tex);;
	t2d.image(0, t2d.ifmt, UNSIGNED_BYTE, data.data());
}

void Texture2D::Loader::storage(PixelFormat fmt, unsigned w, unsigned h, unsigned b)
{
	static_cast<Texture2D &>(tex).storage(fmt, w, h, b);
}

} // namespace GL
} // namespace Msp
