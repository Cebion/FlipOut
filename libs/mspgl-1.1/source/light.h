/* $Id: light.h 80 2009-07-05 20:32:00Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_LIGHT_H_
#define MSP_GL_LIGHT_H_

#include <vector>
#include "color.h"
#include "vector.h"

namespace Msp {
namespace GL {

class Light
{
private:
	Color ambient;
	Color diffuse;
	Color specular;
	Vector4 position;
	Vector3 spot_dir;
	float spot_exp;
	float spot_cutoff;
	float attenuation[3];

	static unsigned current_unit;
	static std::vector<const Light *> current_lights;

public:
	Light();

	void set_ambient(const Color &c);
	void set_diffuse(const Color &c);
	void set_specular(const Color &c);
	const Color &get_ambient() const { return ambient; }
	const Color &get_diffuse() const { return diffuse; }
	const Color &get_specular() const { return specular; }

	void set_position(const Vector4 &);
	const Vector4 &get_position() const { return position; }
	void set_spot_direction(const Vector3 &);
	void set_spot_exponent(float);
	void set_spot_cutoff(float);
	const Vector3 &get_spot_direction() const { return spot_dir; }
	float get_spot_exponent() const { return spot_exp; }
	float get_spot_cutoff() const { return spot_cutoff; }
	void set_attenuation(float, float, float);
	const float *get_attenuation() const { return attenuation; }

	void bind() const;
	void bind_to(unsigned) const;

	// Deprecated
	void set_position(float x, float y, float z, float w) { set_position(Vector4(x, y, z, w)); }

	static void activate(unsigned);
	static void unbind();
};

} // namespace GL
} // namespace Msp

#endif
