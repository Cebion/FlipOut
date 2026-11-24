/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/immediate.h>
#include <msp/gl/matrix.h>
#include <msp/gl/projection.h>
#include "misc.h"
#include "vector3.h"

using namespace Msp;

void build_matrix(const dReal *pos, const dReal *rot, double *matrix)
{
	matrix[ 0]=rot[ 0];  matrix[ 4]=rot[ 1];  matrix[ 8]=rot[ 2];  matrix[12]=pos[ 0];
	matrix[ 1]=rot[ 4];  matrix[ 5]=rot[ 5];  matrix[ 9]=rot[ 6];  matrix[13]=pos[ 1];
	matrix[ 2]=rot[ 8];  matrix[ 6]=rot[ 9];  matrix[10]=rot[10];  matrix[14]=pos[ 2];
	matrix[ 3]=0;        matrix[ 7]=0;        matrix[11]=0;        matrix[15]=1;
}

void get_matrix(dBodyID body, double *matrix)
{
	const dReal *pos=dBodyGetPosition(body);
	const dReal *rot=dBodyGetRotation(body);

	build_matrix(pos, rot, matrix);
}

void get_matrix(dGeomID geom, double *matrix)
{
	const dReal *pos=dGeomGetPosition(geom);
	const dReal *rot=dGeomGetRotation(geom);

	build_matrix(pos, rot, matrix);
}

void draw_fullscreen_quad()
{
	GL::matrix_mode(GL::PROJECTION);
	GL::push_matrix();
	GL::load_identity();
	GL::ortho_bottomleft(1, 1);

	GL::matrix_mode(GL::MODELVIEW);
	GL::push_matrix();
	GL::load_identity();

	GL::Immediate imm((GL::COLOR4_UBYTE, GL::TEXCOORD2, GL::VERTEX2));
	imm.begin(GL::QUADS);
	imm.color(1.0f, 1.0f, 1.0f);
	imm.texcoord(0, 0);
	imm.vertex(0, 0);
	imm.texcoord(1, 0);
	imm.vertex(1, 0);
	imm.texcoord(1, 1);
	imm.vertex(1, 1);
	imm.texcoord(0, 1);
	imm.vertex(0, 1);
	imm.end();

	GL::matrix_mode(GL::PROJECTION);
	GL::pop_matrix();

	GL::matrix_mode(GL::MODELVIEW);
	GL::pop_matrix();
}

void normalize(Vector3 &v)
{
	float l=sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	v.x/=l;
	v.y/=l;
	v.z/=l;
}
