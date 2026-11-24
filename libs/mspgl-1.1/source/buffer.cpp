/* $Id: buffer.cpp 77 2009-05-24 10:56:35Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "arb_vertex_buffer_object.h"
#include "extension.h"
#include "buffer.h"

namespace Msp {
namespace GL {

Buffer::Buffer(BufferType t):
	type(t),
	usage(STATIC_DRAW)
{
	static RequireExtension _req_vbo("GL_ARB_vertex_buffer_object");
	if(type==PIXEL_PACK_BUFFER || type==PIXEL_UNPACK_BUFFER)
		static RequireExtension _req_pbo("GL_ARB_pixel_buffer_object");

	glGenBuffersARB(1, &id);
}

Buffer::~Buffer()
{
	glDeleteBuffersARB(1, &id);
}

void Buffer::bind(BufferType t) const
{
	glBindBufferARB(t, id);
	binding(t)=this;
}

void Buffer::maybe_bind() const
{
	if(binding(type)!=this)
		bind();
}

void Buffer::set_usage(BufferUsage u)
{
	usage=u;
}

void Buffer::data(unsigned size, const void *d)
{
	maybe_bind();
	glBufferDataARB(type, size, d, usage);
}

void Buffer::sub_data(unsigned offset, unsigned size, const void *d)
{
	maybe_bind();
	glBufferSubDataARB(type, offset, size, d);
}

void Buffer::unbind(BufferType type)
{
	const Buffer *&ptr=binding(type);
	if(ptr)
	{
		glBindBufferARB(type, 0);
		ptr=0;
	}
}

const Buffer *&Buffer::binding(BufferType type)
{
	switch(type)
	{
	case ARRAY_BUFFER:         return bound[0];
	case ELEMENT_ARRAY_BUFFER: return bound[1];
	case PIXEL_PACK_BUFFER:    return bound[2];
	case PIXEL_UNPACK_BUFFER:  return bound[3];
	default: throw InvalidParameterValue("Invalid buffer type");
	}
}

const Buffer *Buffer::bound[4]={ 0, 0, 0, 0 };

} // namespace GL
} // namespace Msp
