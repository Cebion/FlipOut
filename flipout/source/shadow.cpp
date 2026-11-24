/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/matrix.h>
#include <msp/gl/misc.h>
#include <msp/gl/projection.h>
#include <msp/gl/texture2d.h>
#include <msp/gl/texunit.h>
#include "flipout.h"
#include "misc.h"
#include "shadow.h"

using namespace Msp;

Shadow::Shadow(FlipOut &, unsigned s, const Vector3 &d, float a, float nd, float xd):
	size(s),
	dir(d),
	area(a),
	min_depth(nd),
	max_depth(xd),
	depth_range(xd-nd),
	framebuf(new GL::Framebuffer),
	tex(new GL::Texture2D)
{
	normalize(dir);

	tex->set_min_filter(GL::NEAREST);
	tex->set_mag_filter(GL::LINEAR);
	tex->parameter(GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	tex->parameter(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	tex->parameter(GL_TEXTURE_COMPARE_FUNC, GL_LESS);
	tex->parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	tex->parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	tex->storage(GL::DEPTH_COMPONENT, size, size, 0);
	tex->image(0, GL::DEPTH_COMPONENT, GL::UNSIGNED_INT, 0);
	framebuf->attach(GL::DEPTH_ATTACHMENT, *tex, 0);
	glDrawBuffer(GL_NONE);
	GL::Framebuffer::unbind();
}

Shadow::~Shadow()
{
	delete framebuf;
	delete tex;
}

void Shadow::create(const GL::Scene &scene)
{
	GL::matrix_mode(GL::PROJECTION);
	GL::push_matrix();
	GL::load_identity();
	GL::ortho(-area/2, area/2, 0, area, min_depth, max_depth);
	GL::matrix_mode(GL::MODELVIEW);
	GL::push_matrix();
	GL::load_identity();
	double matrix[16];
	matrix[ 0]=1;  matrix[ 4]=0;      matrix[ 8]=0;       matrix[12]=0;
	matrix[ 1]=0;  matrix[ 5]=dir.z;  matrix[ 9]=-dir.y;  matrix[13]=0;
	matrix[ 2]=0;  matrix[ 6]=dir.y;  matrix[10]=dir.z;   matrix[14]=0;
	matrix[ 3]=0;  matrix[ 7]=0;      matrix[11]=0;       matrix[15]=1;
	GL::mult_matrix(matrix);

	framebuf->bind();
	glViewport(0, 0, size, size);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	scene.render("shadow");

	GL::Framebuffer::unbind();

	GL::matrix_mode(GL::PROJECTION);
	GL::pop_matrix();
	GL::matrix_mode(GL::MODELVIEW);
	GL::pop_matrix();
}

void Shadow::apply()
{
	float s_eq[4]={ 1.0/area, 0.0, 0.0, 0.5 };
	float t_eq[4]={ 0.0, dir.z/area, -dir.y/area, 0.0 };
	float r_eq[4]={ 0.0, -dir.y/depth_range, -dir.z/depth_range, (-min_depth-0.1)/depth_range };
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, s_eq);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, t_eq);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, r_eq);
	GL::enable(GL_TEXTURE_GEN_S);
	GL::enable(GL_TEXTURE_GEN_T);
	GL::enable(GL_TEXTURE_GEN_R);
	tex->bind();
}
