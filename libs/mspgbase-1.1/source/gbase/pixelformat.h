/* $Id: pixelformat.h 26 2008-08-17 21:34:32Z tdb $

This file is part of libmspgl
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GBASE_PIXELFORMAT_H_
#define MSP_GBASE_PIXELFORMAT_H_

namespace Msp {
namespace Graphics {

enum PixelFormat
{
	COLOR_INDEX,
	LUMINANCE,
	LUMINANCE_ALPHA,
	RGB,
	RGBA,
	BGR,
	BGRA
};

} // namespace Graphics
} // namespace Msp

#endif
