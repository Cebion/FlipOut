/* $Id: vertexarray.h 77 2009-05-24 10:56:35Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_VERTEXARRAY_H_
#define MSP_GL_VERTEXARRAY_H_

#include <vector>
#include <msp/core/refptr.h>
#include <msp/datafile/loader.h>
#include "primitivetype.h"
#include "types.h"
#include "vertexarraybuilder.h"
#include "vertexformat.h"

namespace Msp {
namespace GL {

class Buffer;

class VertexArray
{
public:
	class Loader: public DataFile::Loader, public VertexArrayBuilder
	{
	public:
		Loader(VertexArray &);
	};

private:
	VertexFormat format;
	std::vector<float> data;
	uint         stride;
	Buffer       *vbuf;
	bool         own_vbuf;

	VertexArray(const VertexArray &);
	VertexArray &operator=(const VertexArray &);
public:
	VertexArray(const VertexFormat &);
	~VertexArray();

	const VertexFormat &get_format() const { return format; }
	const std::vector<float> &get_data() const { return data; }
	void         use_vertex_buffer();
	void         use_vertex_buffer(Buffer *);
	void         reserve(unsigned);
	unsigned     size() const { return data.size()/stride; }
	void         clear();
	void         reset(const VertexFormat &);
	void         apply() const;
	void         update_data();
	float        *append();
	float        *operator[](unsigned i) { return &data[0]+i*stride; }
	const float  *operator[](unsigned i) const { return &data[0]+i*stride; }
private:
	void set_array(unsigned, bool, unsigned) const;

	static unsigned enabled_arrays;
};

void array_element(int);
void draw_arrays(PrimitiveType, int, sizei);
void draw_elements(PrimitiveType, sizei, DataType, const void *);
void draw_range_elements(PrimitiveType, uint, uint, sizei, DataType, const void *);

inline void draw_elements(PrimitiveType mode, sizei count, const unsigned *indices)
{ draw_elements(mode, count, UNSIGNED_INT, indices); }

inline void draw_elements(PrimitiveType mode, sizei count, const unsigned short *indices)
{ draw_elements(mode, count, UNSIGNED_SHORT, indices); }

inline void draw_range_elements(PrimitiveType mode, uint low, uint high, sizei count, const unsigned short *indices)
{ draw_range_elements(mode, low, high, count, UNSIGNED_SHORT, indices); }

inline void draw_range_elements(PrimitiveType mode, uint low, uint high, sizei count, const unsigned *indices)
{ draw_range_elements(mode, low, high, count, UNSIGNED_INT, indices); }

} // namespace GL
} // namespace Msp

#endif
