/* $Id: mesh.cpp 77 2009-05-24 10:56:35Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "buffer.h"
#include "mesh.h"

using namespace std;

namespace Msp {
namespace GL {

Mesh::Mesh():
	vertices(VERTEX3),
	ibuf(0)
{ }

Mesh::Mesh(const VertexFormat &f):
	vertices(f),
	ibuf(0)
{ }

void Mesh::use_vertex_buffer(bool b)
{
	if(b)
	{
		vertices.use_vertex_buffer();
		if(!ibuf)
			ibuf=new Buffer(ELEMENT_ARRAY_BUFFER);
		update_index_buffer();
	}
	else
	{
		vertices.use_vertex_buffer(0);
		delete ibuf;
		ibuf=0;
	}
}

float *Mesh::get_vertex(unsigned i)
{
	return vertices[i];
}

void Mesh::add_batch(const Batch &b)
{
	batches.push_back(b);
	update_index_buffer();
}

void Mesh::clear()
{
	vertices.clear();
	batches.clear();
}

void Mesh::draw() const
{
	vertices.apply();
	if(ibuf)
	{
		ibuf->bind();
		unsigned offset=0;
		for(list<Batch>::const_iterator i=batches.begin(); i!=batches.end(); ++i)
		{
			i->draw_with_buffer(offset);
			offset+=i->size();
		}
		ibuf->unbind();
	}
	else
	{
		for(list<Batch>::const_iterator i=batches.begin(); i!=batches.end(); ++i)
			i->draw();
	}
}

void Mesh::update_index_buffer()
{
	if(!ibuf)
		return;

	unsigned total=0;
	for(list<Batch>::const_iterator i=batches.begin(); i!=batches.end(); ++i)
		total+=i->size();

	ibuf->data(total*sizeof(unsigned), 0);
	unsigned offset=0;
	for(list<Batch>::const_iterator i=batches.begin(); i!=batches.end(); ++i)
	{
		ibuf->sub_data(offset*sizeof(unsigned), i->size()*sizeof(unsigned), &i->get_indices()[0]);
		offset+=i->size();
	}
	ibuf->unbind();
}


Mesh::Loader::Loader(Mesh &m):
	mesh(m)
{
	add("vertices", &Loader::vertices);
	add("batch",    &Loader::batch);
}

void Mesh::Loader::vertices(VertexFormat f)
{
	mesh.vertices.reset(f);
	load_sub(mesh.vertices);
}

void Mesh::Loader::batch(PrimitiveType p)
{
	mesh.batches.push_back(Batch(p));
	load_sub(mesh.batches.back());
}

} // namespace GL
} // namespace Msp
