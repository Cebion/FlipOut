/* $Id: image.cpp 40 2009-07-26 18:10:27Z tdb $

This file is part of libmspgbase
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifdef WITH_DEVIL
#include <IL/il.h>
#endif
#ifdef WITH_LIBPNG
#include <png.h>
#include <msp/io/file.h>
#include <msp/io/memory.h>
#endif
#include <msp/core/except.h>
#include "image.h"

using namespace std;

namespace Msp {
namespace Graphics {

struct Image::Private
{
#ifdef WITH_DEVIL
	unsigned id;
#endif
#ifdef WITH_LIBPNG
	PixelFormat fmt;
	unsigned width;
	unsigned height;
	char *data;
#endif

	Private();
};

Image::Private::Private()
{
#ifdef WITH_DEVIL
	id=0;
#endif
#ifdef WITH_LIBPNG
	fmt=RGB;
	width=0;
	height=0;
	data=0;
#endif
}


namespace {

#ifdef WITH_LIBPNG
void read(png_struct *png, png_byte *data, png_size_t size)
{
	IO::Base *in=reinterpret_cast<IO::Base *>(png_get_io_ptr(png));
	in->read(reinterpret_cast<char *>(data), size);
}

void load_png(IO::Base &in, Image::Private &priv)
{
	png_struct *png=0;
	png_info *info=0;
	priv.data=0;

	try
	{
		png=png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
		info=png_create_info_struct(png);

		if(setjmp(png_jmpbuf(png)))
			throw Exception("Error loading PNG image");

		png_set_read_fn(png, &in, read);
		png_read_info(png, info);
		png_uint_32 width;
		png_uint_32 height;
		int depth;
		int color;
		png_get_IHDR(png, info, &width, &height, &depth, &color, 0, 0, 0);
		priv.width=width;
		priv.height=height;
		if(depth!=8)
			throw Exception("Only 8-bit PNG images are supported");
		switch(color)
		{
		case PNG_COLOR_TYPE_PALETTE:    priv.fmt=COLOR_INDEX; break;
		case PNG_COLOR_TYPE_GRAY:       priv.fmt=LUMINANCE; break;
		case PNG_COLOR_TYPE_GRAY_ALPHA: priv.fmt=LUMINANCE_ALPHA; break;
		case PNG_COLOR_TYPE_RGB:        priv.fmt=RGB; break;
		case PNG_COLOR_TYPE_RGB_ALPHA:  priv.fmt=RGBA; break;
		default: throw Exception("Unknown color type");
		}

		unsigned nchans=png_get_channels(png, info);
		if(nchans==4 && priv.fmt==RGB)
			png_set_strip_alpha(png);

		unsigned rowstride=priv.width*nchans;
		priv.data=new char[rowstride*priv.height];
		for(unsigned y=0; y<priv.height; ++y)
			png_read_row(png, reinterpret_cast<png_byte *>(priv.data+rowstride*(priv.height-1-y)), 0);

		png_read_end(png, 0);
		png_destroy_read_struct(&png, &info, 0);
	}
	catch(...)
	{
		png_destroy_read_struct(&png, &info, 0);
		delete[] priv.data;
		throw;
	}
}
#endif

#ifdef WITH_DEVIL
void ensure_devil_image(unsigned &id)
{
	static bool init_done=false;

	if(!init_done)
	{
		ilInit();
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
		init_done=true;
	}

	if(!id)
		ilGenImages(1, &id);
}
#endif

}


Image::Image():
	priv(new Private)
{
#if !defined(WITH_DEVIL) && !defined(WITH_LIBPNG)
	throw Exception("Image needs either DevIL or libpng support");
#endif
}

Image::~Image()
{
#ifdef WITH_DEVIL
	if(priv->id)
		ilDeleteImages(1, &priv->id);
#endif
#ifdef WITH_LIBPNG
	delete[] priv->data;
#endif
	delete priv;
}

void Image::load_file(const string &fn)
{
#ifdef WITH_LIBPNG
	if(fn.size()>4 && !fn.compare(fn.size()-4, 4, ".png"))
	{
		IO::BufferedFile file(fn);
		load_png(file, *priv);
	}
	else
#endif
	{
#ifdef WITH_DEVIL
		ensure_devil_image(priv->id);
		ilBindImage(priv->id);
		if(!ilLoadImage(const_cast<char *>(fn.c_str())))
			throw Exception("Error loading image "+fn);
#else
		throw Exception("Not a PNG image and DevIL support not compiled in");
#endif
	}
	(void)fn;
}

void Image::load_memory(const void *data, unsigned size)
{
#ifdef WITH_LIBPNG
	if(!png_sig_cmp(reinterpret_cast<png_byte *>(const_cast<void *>(data)), 0, 8))
	{
		IO::Memory mem(reinterpret_cast<const char *>(data), size);
		load_png(mem, *priv);
	}
	else
#endif
	{
#ifdef WITH_DEVIL
		ensure_devil_image(priv->id);
		ilBindImage(priv->id);
		if(!ilLoadL(IL_TYPE_UNKNOWN, const_cast<void *>(data), size))
			throw Exception("Error loading image from memory");
#else
		throw Exception("Not a PNG image and DevIL support not compiled in");
#endif
	}
	(void)data;
	(void)size;
}

PixelFormat Image::get_format() const
{
#ifdef WITH_LIBPNG
	if(priv->data)
		return priv->fmt;
#endif
#ifdef WITH_DEVIL
	if(priv->id)
	{
		ilBindImage(priv->id);
		switch(ilGetInteger(IL_IMAGE_FORMAT))
		{
		case IL_COLOR_INDEX: return COLOR_INDEX;
		case IL_LUMINANCE: return LUMINANCE;
		case IL_LUMINANCE_ALPHA: return LUMINANCE_ALPHA;
		case IL_RGB: return RGB;
		case IL_RGBA: return RGBA;
		case IL_BGR: return BGR;
		case IL_BGRA: return BGRA;
		default: throw InvalidParameterValue("Unknown pixel format in image");
		}
	}
#endif
	return RGB;
}

unsigned Image::get_width() const
{
#ifdef WITH_LIBPNG
	if(priv->data)
		return priv->width;
#endif
#ifdef WITH_DEVIL
	if(priv->id)
	{
		ilBindImage(priv->id);
		return ilGetInteger(IL_IMAGE_WIDTH);
	}
#endif
	return 0;
}

unsigned Image::get_height() const
{
#ifdef WITH_LIBPNG
	if(priv->data)
		return priv->height;
#endif
#ifdef WITH_DEVIL
	if(priv->id)
	{
		ilBindImage(priv->id);
		return ilGetInteger(IL_IMAGE_HEIGHT);
	}
#endif
	return 0;
}

const void *Image::get_data() const
{
#ifdef WITH_LIBPNG
	if(priv->data)
		return priv->data;
#endif
#ifdef WITH_DEVIL
	if(priv->id)
	{
		ilBindImage(priv->id);
		return ilGetData();
	}
#endif
	return 0;
}

} // namespace Graphics
} // namespace Msp
