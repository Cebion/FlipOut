/* $Id: light.cpp 80 2009-07-05 20:32:00Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "except.h"
#include "light.h"
#include "misc.h"

using namespace std;

namespace Msp {
namespace GL {

Light::Light():
	ambient(0),
	diffuse(1),
	specular(1),
	position(0, 0, 1, 0),
	spot_dir(0, 0, -1),
	spot_exp(0),
	spot_cutoff(180)
{
	attenuation[0]=1;
	attenuation[1]=0;
	attenuation[2]=0;
}

void Light::set_ambient(const Color &c)
{
	ambient=c;
}

void Light::set_diffuse(const Color &c)
{
	diffuse=c;
}

void Light::set_specular(const Color &c)
{
	specular=c;
}

void Light::set_position(const Vector4 &p)
{
	position=p;
}

void Light::set_spot_direction(const Vector3 &d)
{
	spot_dir=d;
}

void Light::set_spot_exponent(float e)
{
	spot_exp=e;
}

void Light::set_spot_cutoff(float c)
{
	spot_cutoff=c;
}

void Light::set_attenuation(float c, float l, float q)
{
	attenuation[0]=c;
	attenuation[1]=l;
	attenuation[2]=q;
}

void Light::bind() const
{
	if(current_lights[current_unit]!=this)
	{
		GLenum l=GL_LIGHT0+current_unit;
		enable(l);
		glLightfv(l, GL_AMBIENT, &ambient.r);
		glLightfv(l, GL_DIFFUSE, &diffuse.r);
		glLightfv(l, GL_SPECULAR, &specular.r);
		glLightfv(l, GL_POSITION, &position.x);
		glLightfv(l, GL_SPOT_DIRECTION, &spot_dir.x);
		glLightf(l, GL_SPOT_EXPONENT, spot_exp);
		glLightf(l, GL_SPOT_CUTOFF, spot_cutoff);
		glLightf(l, GL_CONSTANT_ATTENUATION, attenuation[0]);
		glLightf(l, GL_LINEAR_ATTENUATION, attenuation[1]);
		glLightf(l, GL_QUADRATIC_ATTENUATION, attenuation[2]);
		current_lights[current_unit]=this;
	}
}

void Light::bind_to(unsigned i) const
{
	activate(i);
	bind();
}

void Light::activate(unsigned i)
{
	static unsigned max_lights=get_i(GL_MAX_LIGHTS);

	if(i>=max_lights)
		throw InvalidParameterValue("Light unit index out of range");

	if(i>=current_lights.size())
		current_lights.resize(i+1);

	current_unit=i;
}

void Light::unbind()
{
	if(current_lights[current_unit])
	{
		disable(GL_LIGHT0+current_unit);
		current_lights[current_unit]=0;
	}
}

unsigned Light::current_unit=0;
vector<const Light *> Light::current_lights(1);

} // namespace GL
} // namespace Msp
