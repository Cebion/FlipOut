/* $Id: meshbuilder.h 49 2008-03-24 00:10:57Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_MESHBUILDER_H_
#define MSP_GL_MESHBUILDER_H_

#include "primitivebuilder.h"

namespace Msp {
namespace GL {

class Batch;
class Mesh;

class MeshBuilder: public PrimitiveBuilder
{
private:
	Mesh  &mesh;
	Batch *batch;

public:
	MeshBuilder(Mesh &);
private:
	virtual void begin_();
	virtual void end_();
	virtual void element_(unsigned);
};

} // namespace GL
} // namespace Msp

#endif
