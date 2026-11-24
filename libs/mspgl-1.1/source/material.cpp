/* $Id: material.cpp 62 2008-09-24 16:10:43Z tdb $

This file is part of libmspgl
Copyright © 2007-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "gl.h"
#include "material.h"

namespace Msp {
namespace GL {

Material::Material():
	ambient(0.2),
	diffuse(0.8),
	specular(0),
	emission(0),
	shininess(0)
{ }

void Material::set_ambient(const Color &a)
{
	ambient=a;
}

void Material::set_diffuse(const Color &d)
{
	diffuse=d;
}

void Material::set_specular(const Color &s)
{
	specular=s;
}

void Material::set_emission(const Color &e)
{
	emission=e;
}

void Material::set_shininess(float s)
{
	shininess=s;
}

void Material::bind() const
{
	if(current!=this)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &ambient.r);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &diffuse.r);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &specular.r);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &emission.r);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
		current=this;
	}
}

void Material::unbind()
{
	current=0;
}

const Material *Material::current=0;


Material::Loader::Loader(Material &m):
	mat(m)
{
	add("ambient",   &Loader::ambient);
	add("diffuse",   &Loader::diffuse);
	add("specular",  &Loader::specular);
	add("emission",  &Loader::emission);
	add("shininess", &Material::shininess);
}

void Material::Loader::ambient(float r, float g, float b, float a)
{
	mat.ambient=GL::Color(r, g, b, a);
}

void Material::Loader::diffuse(float r, float g, float b, float a)
{
	mat.diffuse=GL::Color(r, g, b, a);
}

void Material::Loader::specular(float r, float g, float b, float a)
{
	mat.specular=GL::Color(r, g, b, a);
}

void Material::Loader::emission(float r, float g, float b, float a)
{
	mat.emission=GL::Color(r, g, b, a);
}

} // namespace GL
} // namespace Msp
