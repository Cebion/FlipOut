/* $Id: vertexarraybuilder.h 60 2008-09-05 22:36:05Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_VERTEXARRAYBUIDER_H_
#define MSP_GL_VERTEXARRAYBUIDER_H_

#include <vector>
#include "vertexbuilder.h"
#include "vertexformat.h"

namespace Msp {
namespace GL {

class VertexArray;

class VertexArrayBuilder: public VertexBuilder
{
public:
	VertexArrayBuilder(VertexArray &);
	~VertexArrayBuilder();

private:
	VertexArray  &array;

	VertexArrayBuilder(const VertexArrayBuilder &);
	virtual void vertex_(float, float, float, float);
};

} // namespace GL
} // namespace Msp

#endif
