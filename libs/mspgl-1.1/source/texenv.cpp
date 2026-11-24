/* $Id: texenv.cpp 62 2008-09-24 16:10:43Z tdb $

This file is part of libmspgl
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "texenv.h"
#include "texunit.h"

namespace Msp {
namespace GL {

TexEnv::TexEnv():
	mode(MODULATE)
{ }

void TexEnv::set_mode(TexEnvMode m)
{
	mode=m;
}

void TexEnv::set_color(const Color &c)
{
	color=c;
}

void TexEnv::bind()
{
	if(TexUnit::current().set_texenv(this))
	{
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);
		glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, &color.r);
	}
}

void TexEnv::unbind()
{
	TexUnit::current().set_texenv(0);
}

} // namespace GL
} // namespace Msp
