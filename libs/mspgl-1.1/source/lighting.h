/* $Id: lighting.h 64 2008-09-28 15:40:26Z tdb $

This file is part of libmspgl
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_LIGHTING_H_
#define MSP_GL_LIGHTING_H_

#include <vector>
#include "color.h"
#include "gl.h"

namespace Msp {
namespace GL {

class Light;

enum
{
	LIGHTING = GL_LIGHTING
};

/**
Encapsulates global lighting parameters and a number of individual lights.
*/
class Lighting
{
private:
	Color ambient;
	std::vector<const Light *> lights;

	static const Lighting *current;

public:
	Lighting();

	void set_ambient(const Color &);
	const Color &get_ambient() const { return ambient; }

	void attach(unsigned, const Light &);
	void detach(unsigned);
	void bind() const;

	static void unbind();
};

} // namespace GL
} // namespace Msp

#endif
