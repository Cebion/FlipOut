/* $Id: tag.h 37 2007-12-27 17:12:27Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_TAG_H_
#define MSP_GL_TAG_H_

#include <string>

namespace Msp {
namespace GL {

struct Tag
{
	unsigned id;

	Tag(): id(0) { }
	Tag(const char *);
	Tag(const std::string &s);
};

} // namespace GL
} // namespace Msp

#endif
