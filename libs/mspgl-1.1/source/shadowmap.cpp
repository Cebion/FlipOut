/* $Id: shadowmap.cpp 84 2009-09-21 13:50:58Z tdb $

This file is part of libmspgl
Copyright © 2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cmath>
#include <cstdlib>
#include "light.h"
#include "matrix.h"
#include "misc.h"
#include "projection.h"
#include "scene.h"
#include "shadowmap.h"
#include "texunit.h"

using namespace std;

namespace Msp {
namespace GL {

ShadowMap::ShadowMap(unsigned s, const Scene &c, const Light &l):
	size(s),
	scene(c),
	light(l),
	unit(3),
	radius(1)
{
	depth_buf.set_min_filter(LINEAR);
	depth_buf.storage(DEPTH_COMPONENT, size, size, 0);
	depth_buf.image(0, DEPTH_COMPONENT, UNSIGNED_BYTE, 0);
	depth_buf.parameter(GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	depth_buf.parameter(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	depth_buf.parameter(GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	depth_buf.parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	depth_buf.parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	fbo.attach(DEPTH_ATTACHMENT, depth_buf, 0);
	draw_buffer(NO_BUFFER);
	Framebuffer::unbind();
	Texture::unbind();
}

void ShadowMap::set_target(const Vector3 &t, float r)
{
	target=t;
	radius=r;
}

void ShadowMap::set_texture_unit(unsigned u)
{
	unit=u;
}

void ShadowMap::prepare()
{
	const Vector4 &lpos=light.get_position();
	if(lpos.w)
		throw Exception("Non-directional lights not supported at the moment");

	float matrix[16];
	if(abs(lpos.z)>=abs(lpos.x) && abs(lpos.z)>=abs(lpos.y))
	{
		float d=sqrt(lpos.x*lpos.x+lpos.z*lpos.z);
		matrix[0]=lpos.z/d;
		matrix[4]=0;
		matrix[8]=-lpos.x/d;
		matrix[1]=-lpos.x*lpos.y/d;
		matrix[5]=d;
		matrix[9]=-lpos.z*lpos.y/d;
	}
	else
	{
		float d=sqrt(lpos.x*lpos.x+lpos.y*lpos.y);
		matrix[0]=-lpos.y/d;
		matrix[4]=lpos.x/d;
		matrix[8]=0;
		matrix[1]=-lpos.x*lpos.z/d;
		matrix[5]=-lpos.y*lpos.z/d;
		matrix[9]=d;
	}

	matrix[2]=lpos.x;
	matrix[6]=lpos.y;
	matrix[10]=lpos.z;

	matrix[12]=-(target.x*matrix[0]+target.y*matrix[4]+target.z*matrix[8]);
	matrix[13]=-(target.x*matrix[1]+target.y*matrix[5]+target.z*matrix[9]);
	matrix[14]=-(target.x*matrix[2]+target.y*matrix[6]+target.z*matrix[10]);
	matrix[3]=0;
	matrix[7]=0;
	matrix[11]=0;
	matrix[15]=1;

	{
		matrix_mode(PROJECTION);
		push_matrix();
		load_identity();
		ortho(-radius, radius, -radius, radius, -radius, radius);
		matrix_mode(MODELVIEW);
		push_matrix();
		load_matrix(matrix);

		const Framebuffer *old_fbo=Framebuffer::current();
		fbo.bind();
		clear(DEPTH_BUFFER_BIT);
		scene.render("shadow");

		matrix_mode(PROJECTION);
		pop_matrix();
		matrix_mode(MODELVIEW);
		pop_matrix();
		if(old_fbo)
			old_fbo->bind();
		else
			Framebuffer::unbind();
	}

	depth_buf.bind_to(unit);
	float diam=radius*2;
	float s_eq[4]={ matrix[0]/diam, matrix[4]/diam, matrix[8]/diam, matrix[12]/diam+0.5 };
	float t_eq[4]={ matrix[1]/diam, matrix[5]/diam, matrix[9]/diam, matrix[13]/diam+0.5 };
	float r_eq[4]={ -matrix[2]/diam, -matrix[6]/diam, -matrix[10]/diam, 0.5-matrix[14]/diam-4.0/size };
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, s_eq);
	enable(GL_TEXTURE_GEN_S);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, t_eq);
	enable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, r_eq);
	enable(GL_TEXTURE_GEN_R);

	TexUnit::activate(0);
}

void ShadowMap::cleanup()
{
	TexUnit::activate(unit);
	Texture::unbind();
	disable(GL_TEXTURE_GEN_S);
	disable(GL_TEXTURE_GEN_T);
	disable(GL_TEXTURE_GEN_R);
	TexUnit::activate(0);
}

} // namespace GL
} // namespace Msp
