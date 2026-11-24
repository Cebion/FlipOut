/* $Id: vector.h 80 2009-07-05 20:32:00Z tdb $

This file is part of libmspgl
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_VECTOR_H_
#define MSP_GL_VECTOR_H_

namespace Msp {
namespace GL {

struct Vector3
{
	float x, y, z;

	Vector3(): x(0), y(0), z(0) { }
	Vector3(float x_, float y_, float z_): x(x_), y(y_), z(z_) { }
};

struct Vector4
{
	float x, y, z, w;

	Vector4(): x(0), y(0), z(0), w(1) { }
	Vector4(float x_, float y_, float z_): x(x_), y(y_), z(z_), w(1) { }
	Vector4(float x_, float y_, float z_, float w_): x(x_), y(y_), z(z_), w(w_) { }
	Vector4(const Vector3 &v): x(v.x), y(v.y), z(v.z), w(1) { }
};

} // namespace GL
} // namespace Msp

#endif
