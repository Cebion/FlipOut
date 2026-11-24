/* $Id: buffer.h 77 2009-05-24 10:56:35Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_BUFFER_H_
#define MSP_GL_BUFFER_H_

#include "types.h"

namespace Msp {
namespace GL {

enum BufferType
{
	ARRAY_BUFFER         = GL_ARRAY_BUFFER_ARB,
	ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER_ARB,
	PIXEL_PACK_BUFFER    = GL_PIXEL_PACK_BUFFER_ARB,
	PIXEL_UNPACK_BUFFER  = GL_PIXEL_UNPACK_BUFFER_ARB
};

enum BufferUsage
{
	STREAM_DRAW  = GL_STREAM_DRAW_ARB,
	STREAM_READ  = GL_STREAM_READ_ARB,
	STREAM_COPY  = GL_STREAM_COPY_ARB,
	STATIC_DRAW  = GL_STATIC_DRAW_ARB,
	STATIC_READ  = GL_STATIC_READ_ARB,
	STATIC_COPY  = GL_STATIC_COPY_ARB,
	DYNAMIC_DRAW = GL_DYNAMIC_DRAW_ARB,
	DYNAMIC_READ = GL_DYNAMIC_READ_ARB,
	DYNAMIC_COPY = GL_DYNAMIC_COPY_ARB
};

/**
A buffer for storing data in GL memory.  Putting vertex and index data in
buffers can improve rendering performance.  The VertexArray and Mesh classes
contain built-in support for buffers.
*/
class Buffer
{
private:
	BufferType type;
	BufferUsage usage;
	unsigned id;

	static const Buffer *bound[4];

public:
	Buffer(BufferType);
	~Buffer();

	/** Binds the buffer in its default slot. */
	void bind() const { bind(type); }

	/** Binds the buffer in an alternate slot. */
	void bind(BufferType) const;

private:
	void maybe_bind() const;

public:
	/** Unbinds the buffer from its default slot. */
	void unbind() const { unbind(type); }

	/** Sets the usage hint of the buffer.  It will take effect the next time
	the buffer's contents are defined. */
	void set_usage(BufferUsage);

	/** Uploads data into the buffer, completely replacing any previous
	contents. */
	void data(unsigned, const void *);

	/** Overwrites part of the buffer data with new data.  The buffer size can
	not be changed with this call. */
	void sub_data(unsigned, unsigned, const void *);

	static void unbind(BufferType);
private:
	static const Buffer *&binding(BufferType);
};

} // namespace GL
} // namespace Msp

#endif
