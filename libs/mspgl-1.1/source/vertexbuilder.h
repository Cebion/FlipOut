/* $Id: vertexbuilder.h 72 2009-01-10 10:07:36Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_VERTEXBUILDER_H_
#define MSP_GL_VERTEXBUILDER_H_

#include <map>
#include "types.h"

namespace Msp {
namespace GL {

/**
Base class for classes that build vertices from a series of function calls.
The operating model closely follows that of OpenGL immediate mode: vertex
attributes can be specified at any time, and when a vertex() function is
called, a vertex is created with the active attribute values.

A derived class must overload the 4-argument vertex_() function to process the
data.  Attributes can be read from protected member variables.
*/
class VertexBuilder
{
protected:
	struct Attrib
	{
		float x, y, z, w;
	};

public:
	VertexBuilder();
	virtual ~VertexBuilder() { }

	void vertex(float x, float y)                     { vertex(x, y, 0, 1); }
	void vertex(float x, float y, float z)            { vertex(x, y, z, 1); }
	void vertex(float x, float y, float z, float w)   { vertex_(x, y, z, w); }
	void normal(float x, float y, float z)            { nx=x; ny=y; nz=z; }
	void texcoord(float s)                            { texcoord(s, 0, 0, 1); }
	void texcoord(float s, float t)                   { texcoord(s, t, 0, 1); }
	void texcoord(float s, float t, float r)          { texcoord(s, t, r, 1); }
	void texcoord(float s, float t, float r, float q) { ts=s; tt=t; tr=r; tq=q; }
	void color(ubyte r, ubyte g, ubyte b)             { color(r, g, b, 255); }
	void color(ubyte r, ubyte g, ubyte b, ubyte a)    { color(r/255.f, g/255.f, b/255.f, a/255.f); }
	void color(float r, float g, float b)             { color(r, g, b, 1); }
	void color(float r, float g, float b, float a)    { cr=r; cg=g; cb=b; ca=a; }
	void attrib(unsigned i, float x)                  { attrib(i, x, 0, 0, 1); }
	void attrib(unsigned i, float x, float y)         { attrib(i, x, y, 0, 1); }
	void attrib(unsigned i, float x, float y, float z) { attrib(i, x, y, z, 1); }
	void attrib(unsigned i, float x, float y, float z, float w);
protected:
	float cr, cg, cb, ca;  // Color
	float ts, tt, tr, tq;  // TexCoord
	float nx, ny, nz;     // Normal
	std::map<unsigned, Attrib> av;

	virtual void vertex_(float, float, float, float) =0;
};

} // namespace GL
} // namespace Msp

#endif
