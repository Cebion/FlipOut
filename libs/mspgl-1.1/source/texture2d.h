/* $Id: texture2d.h 55 2008-08-17 22:16:15Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_TEXTURE2D_H_
#define MSP_GL_TEXTURE2D_H_

#include <string>
#include <msp/datafile/loader.h>
#include <msp/gbase/image.h>
#include "pixelformat.h"
#include "texture.h"

namespace Msp {
namespace GL {

/**
Two-dimensional texture class.  This is the most common type of texture.
*/
class Texture2D: public Texture
{
public:
	class Loader: public Texture::Loader
	{
	public:
		Loader(Texture2D &);
	private:
		void image_data(const std::string &);
		void raw_data(const std::string &);
		void storage(PixelFormat, unsigned, unsigned, unsigned);
	};

private:
	PixelFormat ifmt;
	sizei width;
	sizei height;
	int border;

public:
	Texture2D();

	/**
	Defines the texture storage.  This function may only be successfully called
	once.
	*/
	void storage(PixelFormat fmt, sizei wd, sizei ht, int brd);
	
	/**
	Uploads an image to the texture.  storage() must have been called prior to
	this, and the image must have dimensions conforming to the specified
	storage.
	*/
	void image(int level, PixelFormat fmt, DataType type, const void *data);

	/**
	Uploads a sub-image into the texture.  Unlike full image upload, there are
	no constraints on the size of the sub-image.
	*/
	void sub_image(int level, int x, int y, sizei wd, sizei ht, PixelFormat fmt, DataType type, const void *data);

	/**
	Loads an image from a file and uploads it to the texture.  If storage() has
	not been called, the storage format will be set to match the loaded image.
	*/
	void load_image(const std::string &fn);

	sizei get_width() const  { return width; }
	sizei get_height() const { return height; }

private:
	void image(const Graphics::Image &);
};

} // namespace GL
} // namespace Msp

#endif
