/* $Id: matrix.h 80 2009-07-05 20:32:00Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_MATRIX_H_
#define MSP_GL_MATRIX_H_

#include "gl.h"

namespace Msp {
namespace GL {

enum MatrixMode
{
	MODELVIEW=GL_MODELVIEW,
	PROJECTION=GL_PROJECTION,
	TEXTURE=GL_TEXTURE
};

void matrix_mode(MatrixMode);
void load_identity();
void load_matrix(const float *);
void load_matrix(const double *);
void mult_matrix(const float *);
void mult_matrix(const double *);
void push_matrix();
void pop_matrix();

/// RAII object - pushes matrix when constructed and pops when destroyed
struct PushMatrix
{
	PushMatrix() { push_matrix(); }
	~PushMatrix() { pop_matrix(); }
};

void translate(float, float, float);
void rotate(float, float, float, float);
void scale(float, float, float);
void scale_uniform(float);

} // namespace GL
} // namespace Msp

#endif
