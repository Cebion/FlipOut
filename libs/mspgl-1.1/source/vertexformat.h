/* $Id: vertexformat.h 72 2009-01-10 10:07:36Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_VERTEXFORMAT_H_
#define MSP_GL_VERTEXFORMAT_H_

#include <istream>
#include "types.h"

namespace Msp {
namespace GL {

enum VertexComponent
{
	VERTEX2=1,
	VERTEX3,
	VERTEX4,
	NORMAL3=6,
	TEXCOORD1=8,
	TEXCOORD2,
	TEXCOORD3,
	TEXCOORD4,
	COLOR4_UBYTE=12,
	COLOR3_FLOAT=14,
	COLOR4_FLOAT,
	ATTRIB1=16,
	ATTRIB2,
	ATTRIB3,
	ATTRIB4
};

struct VertexFormat
{
	unsigned char *data;

	VertexFormat();
	VertexFormat(VertexComponent);
	VertexFormat(const VertexFormat &);
	VertexFormat &operator=(const VertexFormat &);
	~VertexFormat();

	bool empty() const { return !data || !data[0]; }
	const unsigned char *begin() const { return data ? data+1 : 0; }
	const unsigned char *end() const { return data ? data+1+data[0] : 0; }
};

VertexFormat operator,(const VertexFormat &f, VertexComponent c);
inline VertexFormat operator,(VertexComponent c1, VertexComponent c2)
{ return (VertexFormat(c1), c2); }

VertexFormat operator,(const VertexFormat &f, unsigned i);
inline VertexFormat operator,(VertexComponent c, unsigned i)
{ return (VertexFormat(c), i); }

uint get_stride(const VertexFormat &);
std::istream &operator>>(std::istream &, VertexFormat &);

} // namespace GL
} // namespace Msp

#endif
