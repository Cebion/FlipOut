/* $Id: texunit.cpp 62 2008-09-24 16:10:43Z tdb $

This file is part of libmspgl
Copyright © 2007-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "extension.h"
#include "gl.h"
#include "texunit.h"
#include "version_1_3.h"

using namespace std;

namespace Msp {
namespace GL {

TexUnit::TexUnit():
	texture(0)
{
}

bool TexUnit::set_texture(const Texture *tex)
{
	bool result=(tex!=texture);
	texture=tex;
	return result;
}

bool TexUnit::set_texenv(const TexEnv *env)
{
	bool result=(texenv!=env);
	texenv=env;
	return result;
}

TexUnit &TexUnit::activate(unsigned n)
{
	if(units.size()<=n)
		units.resize(n+1);

	if(cur_unit!=&units[n] && (cur_unit || n))
	{
		static RequireVersion _ver(1, 3);
		glActiveTexture(GL_TEXTURE0+n);
	}
	cur_unit=&units[n];

	return units[n];
}

TexUnit &TexUnit::current()
{
	if(!cur_unit)
		return activate(0);
	return *cur_unit;
}

vector<TexUnit> TexUnit::units;
TexUnit *TexUnit::cur_unit=0;

} // namespace GL
} // namespace Msp
