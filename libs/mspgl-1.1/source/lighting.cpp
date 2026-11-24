/* $Id: lighting.cpp 64 2008-09-28 15:40:26Z tdb $

This file is part of libmspgl
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "light.h"
#include "lighting.h"
#include "misc.h"

namespace Msp {
namespace GL {

Lighting::Lighting():
	ambient(0.2)
{ }

void Lighting::set_ambient(const Color &a)
{
	ambient=a;
}

void Lighting::attach(unsigned i, const Light &l)
{
	if(i>=lights.size())
		lights.resize(i+1);

	lights[i]=&l;
}

void Lighting::detach(unsigned i)
{
	if(i>=lights.size())
		return;

	lights[i]=0;
}

void Lighting::bind() const
{
	if(current!=this)
	{
		enable(LIGHTING);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &ambient.r);
		for(unsigned i=0; i<lights.size(); ++i)
			if(lights[i])
				lights[i]->bind_to(i);
		current=this;
	}
}

void Lighting::unbind()
{
	if(current)
	{
		for(unsigned i=0; i<current->lights.size(); ++i)
			if(current->lights[i])
			{
				Light::activate(i);
				Light::unbind();
			}
		disable(LIGHTING);
		current=0;
	}
}

const Lighting *Lighting::current=0;

} // namespace GL
} // namespace Msp
