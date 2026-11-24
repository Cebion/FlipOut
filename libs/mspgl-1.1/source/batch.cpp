/* $Id: batch.cpp 77 2009-05-24 10:56:35Z tdb $

This file is part of libmspgl
Copyright © 2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "batch.h"
#include "extension.h"
#include "vertexarray.h"

using namespace std;

namespace Msp {
namespace GL {

Batch::Batch(PrimitiveType t):
	type(t),
	min_index(0),
	max_index(0)
{ }

Batch &Batch::append(uint i)
{
	if(indices.empty())
		min_index=max_index=i;
	else
	{
		min_index=min(min_index, i);
		max_index=max(max_index, i);
	}
	indices.push_back(i);

	return *this;
}

void Batch::append(const vector<uint> &ind)
{
	indices.reserve(indices.size()+ind.size());
	for(vector<uint>::const_iterator i=ind.begin(); i!=ind.end(); ++i)
		append(*i);
}

void Batch::draw() const
{
	draw_range_elements(type, min_index, max_index, indices.size(), &indices[0]);
}

void Batch::draw_with_buffer(unsigned offset) const
{
	draw_range_elements(type, min_index, max_index, indices.size(), (unsigned *)0+offset);
}


Batch::Loader::Loader(Batch &b):
	batch(b)
{
	add("indices", &Loader::indices);
}

void Batch::Loader::indices(const vector<uint> &ind)
{
	batch.append(ind);
}

} // namespace GL
} // namespace Msp
