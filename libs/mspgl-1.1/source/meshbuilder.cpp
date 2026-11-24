/* $Id: meshbuilder.cpp 49 2008-03-24 00:10:57Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "mesh.h"
#include "meshbuilder.h"

namespace Msp {
namespace GL {

MeshBuilder::MeshBuilder(Mesh &m):
	PrimitiveBuilder(m.vertices),
	mesh(m),
	batch(0)
{ }

void MeshBuilder::begin_()
{
	batch=new Batch(type);
}

void MeshBuilder::end_()
{
	mesh.add_batch(*batch);
	delete batch;
	batch=0;
}

void MeshBuilder::element_(unsigned i)
{
	batch->append(i);
}

} // namespace GL
} // namespace Msp
