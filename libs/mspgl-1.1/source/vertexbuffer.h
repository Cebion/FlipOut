/* $Id: vertexbuffer.h 77 2009-05-24 10:56:35Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_VERTEXBUFFER_H_
#define MSP_GL_VERTEXBUFFER_H_

#include "buffer.h"

namespace Msp {
namespace GL {

/**
Deprecated.  Equivalent to Buffer of type ARRAY_BUFFER.  Retained for backwards
compatibility only.
*/
class VertexBuffer: public Buffer
{
public:
	VertexBuffer(): Buffer(ARRAY_BUFFER) { }

	static void unbind() { Buffer::unbind(ARRAY_BUFFER); }
};

} // namespace GL
} // namespace Msp

#endif
