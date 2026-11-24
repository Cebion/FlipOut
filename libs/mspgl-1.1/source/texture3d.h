/* $Id: texture3d.h 44 2008-02-26 10:28:32Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_TEXTURE3D_H_
#define MSP_GL_TEXTURE3D_H_

#include <string>
#include "pixelformat.h"
#include "texture.h"

namespace Msp {
namespace GL {

class Texture3D: public Texture
{
private:
	PixelFormat ifmt;
	sizei width;
	sizei height;
	sizei depth;
	int border;

public:
	Texture3D();
	void storage(PixelFormat, sizei, sizei, sizei, int);
	void image(int, PixelFormat, DataType, const void *);
	void sub_image(int, int, int, sizei, sizei, sizei, PixelFormat, DataType, const void *);
	void load_image(const std::string &fn, int dp=-1);
	sizei get_width() const { return width; }
	sizei get_height() const { return height; }
	sizei get_depth() const { return depth; }
};

} // namespace GL
} // namespace Msp

#endif
