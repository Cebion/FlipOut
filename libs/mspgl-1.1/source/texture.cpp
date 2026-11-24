/* $Id: texture.cpp 65 2008-10-04 19:50:15Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "except.h"
#include "texture.h"
#include "texunit.h"

using namespace std;

namespace Msp {
namespace GL {

istream &operator>>(istream &in, TextureFilter &tf)
{
	string str;
	in>>str;

	if(str=="NEAREST")
		tf=NEAREST;
	else if(str=="LINEAR")
		tf=LINEAR;
	else if(str=="NEAREST_MIPMAP_NEAREST")
		tf=NEAREST_MIPMAP_NEAREST;
	else if(str=="NEAREST_MIPMAP_LINEAR")
		tf=NEAREST_MIPMAP_LINEAR;
	else if(str=="LINEAR_MIPMAP_NEAREST")
		tf=LINEAR_MIPMAP_NEAREST;
	else if(str=="LINEAR_MIPMAP_LINEAR")
		tf=LINEAR_MIPMAP_LINEAR;
	else
		in.setstate(ios_base::failbit);

	return in;
}


void Texture::bind() const
{
	if(!target)
		throw InvalidState("Attempt to bind a texture without target");

	const Texture *cur=TexUnit::current().get_texture();
	if(cur!=this)
	{
		if(cur && cur->target!=target)
			glDisable(cur->target);
		if(!cur || cur->target!=target)
			glEnable(target);
		glBindTexture(target, id);
		TexUnit::current().set_texture(this);
	}
}

void Texture::bind_to(unsigned i) const
{
	TexUnit::activate(i);
	bind();
}

void Texture::parameter(GLenum param, int value)
{
	maybe_bind();

	glTexParameteri(target, param, value);
}

void Texture::parameter(GLenum param, float value)
{
	maybe_bind();

	glTexParameterf(target, param, value);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

void Texture::unbind()
{
	const Texture *cur=TexUnit::current().get_texture();
	if(!cur)
		return;

	glBindTexture(cur->target, 0);
	glDisable(cur->target);
	TexUnit::current().set_texture(0);
}

Texture::Texture():
	target(0)
{
	glGenTextures(1, &id);
}

void Texture::maybe_bind() const
{
	if(TexUnit::current().get_texture()!=this)
		bind();
}


Texture::Loader::Loader(Texture &t):
	tex(t)
{
	add("min_filter", &Loader::min_filter);
	add("mag_filter", &Loader::mag_filter);
	add("generate_mipmap", &Loader::generate_mipmap);
}

void Texture::Loader::min_filter(TextureFilter f)
{
	tex.set_min_filter(f);
}

void Texture::Loader::mag_filter(TextureFilter f)
{
	tex.set_mag_filter(f);
}

void Texture::Loader::generate_mipmap(bool gm)
{
	tex.parameter(GL_GENERATE_MIPMAP_SGIS, gm);
}

} // namespace GL
} // namespace Msp
