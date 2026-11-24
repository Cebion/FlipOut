/* $Id: image.h 37 2009-01-23 19:47:37Z tdb $

This file is part of libmspgbase
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GBASE_IMAGE_H_
#define MSP_GBASE_IMAGE_H_

#include <string>
#include "pixelformat.h"

namespace Msp {
namespace Graphics {

class Image
{
public:
	struct Private;

private:
	Private *priv;

public:
	Image();
	~Image();

	void load_file(const std::string &);
	void load_memory(const void *, unsigned);
	PixelFormat get_format() const;
	unsigned get_width() const;
	unsigned get_height() const;
	const void *get_data() const;
};

} // namespace Graphics
} // namespace Msp

#endif
