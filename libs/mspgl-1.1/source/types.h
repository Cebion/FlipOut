/* $Id: types.h 43 2008-02-06 10:43:42Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_TYPES_H_
#define MSP_GL_TYPES_H_

#include "gl.h"

namespace Msp {
namespace GL {

enum DataType
{
	BYTE           = GL_BYTE,
	UNSIGNED_BYTE  = GL_UNSIGNED_BYTE,
	SHORT          = GL_SHORT,
	UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
	INT            = GL_INT,
	UNSIGNED_INT   = GL_UNSIGNED_INT,
	FLOAT          = GL_FLOAT,
	DOUBLE         = GL_DOUBLE
};

typedef signed char byte;
typedef unsigned char ubyte;
typedef unsigned sizei;
typedef unsigned uint;

} // namespace GL
} // namespace Msp

#endif
