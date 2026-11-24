/* $Id: mesh.h 77 2009-05-24 10:56:35Z tdb $

This file is part of libmspgl
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_MESH_H_
#define MSP_GL_MESH_H_

#include <msp/datafile/loader.h>
#include "batch.h"
#include "vertexarray.h"

namespace Msp {
namespace GL {

class Buffer;

class Mesh
{
	friend class MeshBuilder;

public:
	class Loader: public DataFile::Loader
	{
	public:
		Loader(Mesh &);
	private:
		Mesh &mesh;

		void vertices(VertexFormat);
		void batch(PrimitiveType);
	};

private:
	VertexArray vertices;
	std::list<Batch> batches;
	Buffer *ibuf;

public:
	Mesh();
	Mesh(const VertexFormat &f);

	void use_vertex_buffer(bool);
	const VertexArray &get_vertices() const { return vertices; }
	float *get_vertex(unsigned);
	void add_batch(const Batch &b);
	const std::list<Batch> &get_batches() { return batches; }
	void clear();
	void draw() const;
private:
	void update_index_buffer();
};

} // namespace GL
} // namespace Msp

#endif
