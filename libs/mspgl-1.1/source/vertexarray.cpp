/* $Id: vertexarray.cpp 77 2009-05-24 10:56:35Z tdb $

This file is part of libmspgl
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "arb_vertex_program.h"
#include "extension.h"
#include "gl.h"
#include "version_1_2.h"
#include "vertexarray.h"
#include "vertexbuffer.h"

using namespace std;

namespace Msp {
namespace GL {

VertexArray::VertexArray(const VertexFormat &f):
	vbuf(0),
	own_vbuf(false)
{
	reset(f);
}

VertexArray::~VertexArray()
{
	if(own_vbuf)
		delete vbuf;
}

void VertexArray::use_vertex_buffer()
{
	if(vbuf && own_vbuf)
		return;

	vbuf=new Buffer(ARRAY_BUFFER);
	own_vbuf=true;

	update_data();
}

void VertexArray::use_vertex_buffer(Buffer *b)
{
	if(own_vbuf)
		delete vbuf;
	vbuf=b;
	own_vbuf=false;

	update_data();
}

void VertexArray::reserve(unsigned n)
{
	data.reserve(n*stride);
}

void VertexArray::clear()
{
	data.clear();
}

void VertexArray::reset(const VertexFormat &f)
{
	clear();
	format=f;
	stride=get_stride(format);

	bool has_gen_attrs=false;
	for(const unsigned char *c=format.begin(); (!has_gen_attrs && c!=format.end()); ++c)
		has_gen_attrs=(*c>=ATTRIB1);
	if(has_gen_attrs)
		static RequireExtension _ext("GL_ARB_vertex_program");
}

void VertexArray::apply() const
{
	if(format.empty())
		throw InvalidState("Trying to apply a vertex array with no data");

	if(vbuf)
		vbuf->bind();

	const float *base=vbuf?0:&data[0];
	uint offset=0;
	uint found=0;
	uint bpv=stride*sizeof(float);
	for(const unsigned char *c=format.begin(); c!=format.end(); ++c)
	{
		uint sz=(*c&3)+1;
		uint t=*c>>2;
		switch(t)
		{
		case 0:
			glVertexPointer(sz, GL_FLOAT, bpv, base+offset);
			break;
		case 1:
			glNormalPointer(GL_FLOAT, bpv, base+offset);
			break;
		case 2:
			glTexCoordPointer(sz, GL_FLOAT, bpv, base+offset);
			break;
		case 3:
			if(sz==1)
				glColorPointer(4, GL_UNSIGNED_BYTE, bpv, base+offset);
			else
				glColorPointer(sz, GL_FLOAT, bpv, base+offset);
			break;
		default:
			glVertexAttribPointerARB(t-4, sz, GL_FLOAT, false, bpv, base+offset);
			break;
		}
		found|=1<<t;
		offset+=sz;
	}

	set_array(GL_VERTEX_ARRAY, found&1, 1);
	set_array(GL_NORMAL_ARRAY, found&2, 2);
	set_array(GL_TEXTURE_COORD_ARRAY, found&4, 4);
	set_array(GL_COLOR_ARRAY, found&8, 8);
	for(unsigned i=4; i<32; ++i)
		set_array(i-4, (found>>i)&1, 1<<i);

	if(vbuf)
		vbuf->unbind();
}

/**
Updates the VertexArray data to the VertexBuffer tied to the array, if any.
*/
void VertexArray::update_data()
{
	if(vbuf)
	{
		vbuf->data(data.size()*sizeof(float), &data[0]);
		vbuf->unbind();
	}
}

float *VertexArray::append()
{
	data.insert(data.end(), stride, 0.0f);
	return &*data.end()-stride;
}

void VertexArray::set_array(unsigned array, bool en, unsigned mask) const
{
	if((enabled_arrays&mask) && !en)
	{
		if(mask<16)
			glDisableClientState(array);
		else
			glDisableVertexAttribArrayARB(array);
		enabled_arrays&=~mask;
	}
	else if(!(enabled_arrays&mask) && en)
	{
		if(mask<16)
			glEnableClientState(array);
		else
			glEnableVertexAttribArrayARB(array);
		enabled_arrays|=mask;
	}
}

unsigned VertexArray::enabled_arrays=0;


VertexArray::Loader::Loader(VertexArray &a):
	VertexArrayBuilder(a)
{
	add("vertex2",   static_cast<void (Loader::*)(float, float)>(&Loader::vertex));
	add("vertex3",   static_cast<void (Loader::*)(float, float, float)>(&Loader::vertex));
	add("vertex4",   static_cast<void (Loader::*)(float, float, float, float)>(&Loader::vertex));
	add("normal3",   static_cast<void (Loader::*)(float, float, float)>(&Loader::normal));
	add("texcoord1", static_cast<void (Loader::*)(float)>(&Loader::texcoord));
	add("texcoord2", static_cast<void (Loader::*)(float, float)>(&Loader::texcoord));
	add("texcoord3", static_cast<void (Loader::*)(float, float, float)>(&Loader::texcoord));
	add("texcoord4", static_cast<void (Loader::*)(float, float, float, float)>(&Loader::texcoord));
	add("color3",    static_cast<void (Loader::*)(float, float, float)>(&Loader::color));
	add("color4",    static_cast<void (Loader::*)(float, float, float, float)>(&Loader::color));
	add("attrib1",   static_cast<void (Loader::*)(unsigned, float)>(&Loader::attrib));
	add("attrib2",   static_cast<void (Loader::*)(unsigned, float, float)>(&Loader::attrib));
	add("attrib3",   static_cast<void (Loader::*)(unsigned, float, float, float)>(&Loader::attrib));
	add("attrib4",   static_cast<void (Loader::*)(unsigned, float, float, float, float)>(&Loader::attrib));
}


void array_element(int i)
{
	glArrayElement(i);
}

void draw_arrays(PrimitiveType mode, int first, sizei count)
{
	glDrawArrays(mode, first, count);
}

void draw_elements(PrimitiveType mode, sizei count, DataType type, const void *indices)
{
	glDrawElements(mode, count, type, indices);
}

void draw_range_elements(PrimitiveType mode, uint low, uint high, sizei count, DataType type, const void *indices)
{
	static RequireVersion _ver(1, 2);
	glDrawRangeElements(mode, low, high, count, type, indices);
}

} // namespace GL
} // namespace Msp
