/* $Id: vertexarraybuilder.cpp 73 2009-01-14 19:19:18Z tdb $

This file is part of libmspgl
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "vertexarray.h"
#include "vertexarraybuilder.h"

namespace Msp {
namespace GL {

VertexArrayBuilder::VertexArrayBuilder(VertexArray &a):
	array(a)
{ }

VertexArrayBuilder::~VertexArrayBuilder()
{
	array.update_data();
}

void VertexArrayBuilder::vertex_(float x, float y, float z, float w)
{
	float *ptr=array.append();
	for(const unsigned char *c=array.get_format().begin(); c!=array.get_format().end(); ++c)
	{
		uint size=(*c&3)+1;
		uint type=*c>>2;
		switch(type)
		{
		case 0:
			*ptr++=x;
			*ptr++=y;
			if(size>=3) *ptr++=z;
			if(size>=4) *ptr++=w;
			break;
		case 1:
			*ptr++=nx;
			*ptr++=ny;
			*ptr++=nz;
			break;
		case 2:
			*ptr++=ts;
			if(size>=2) *ptr++=tt;
			if(size>=3) *ptr++=tr;
			if(size>=4) *ptr++=tq;
			break;
		case 3:
			if(size==1)
			{
				union { ubyte c[4]; float f; } u;
				u.c[0]=static_cast<ubyte>(cr*255);
				u.c[1]=static_cast<ubyte>(cg*255);
				u.c[2]=static_cast<ubyte>(cb*255);
				u.c[3]=static_cast<ubyte>(ca*255);
				*ptr++=u.f;
			}
			else
			{
				*ptr++=cr;
				*ptr++=cg;
				*ptr++=cb;
				if(size>=4) *+ptr++=ca;
			}
			break;
		default:
			const Attrib &a=av[type-4];
			*ptr++=a.x;
			if(size>=2) *ptr++=a.y;
			if(size>=3) *ptr++=a.z;
			if(size>=4) *ptr++=a.w;
			break;
		}
	}
}

} // namespace GL
} // namespace Msp
