/* $Id: matrix.cpp 80 2009-07-05 20:32:00Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "matrix.h"

namespace Msp {
namespace GL {

void matrix_mode(MatrixMode m)
{
	glMatrixMode(m);
}

void load_identity()
{
	glLoadIdentity();
}

void load_matrix(const float *matrix)
{
	glLoadMatrixf(matrix);
}

void load_matrix(const double *matrix)
{
	glLoadMatrixd(matrix);
}

void mult_matrix(const float *matrix)
{
	glMultMatrixf(matrix);
}

void mult_matrix(const double *matrix)
{
	glMultMatrixd(matrix);
}

void push_matrix()
{
	glPushMatrix();
}

void pop_matrix()
{
	glPopMatrix();
}

void translate(float x, float y, float z)
{
	glTranslatef(x, y, z);
}

void rotate(float a, float x, float y, float z)
{
	glRotatef(a, x, y, z);
}

void scale(float x, float y, float z)
{
	glScalef(x, y, z);
}

void scale_uniform(float s)
{
	scale(s, s, s);
}

} // namespace GL
} // namespace Msp
