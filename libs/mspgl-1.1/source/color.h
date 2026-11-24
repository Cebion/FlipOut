/* $Id: color.h 38 2007-12-29 20:47:08Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_COLOR_H_
#define MSP_GL_COLOR_H_

namespace Msp {
namespace GL {

struct Color
{
	float r, g, b, a;

	Color(): r(1), g(1), b(1), a(1) { }
	Color(float v): r(v), g(v), b(v), a(1) { }
	Color(float r_, float g_, float b_): r(r_), g(g_), b(b_), a(1) { }
	Color(float r_, float g_, float b_, float a_): r(r_), g(g_), b(b_), a(a_) { }
	Color operator*(float f) const { return Color(r*f, g*f, b*f, a); }
	Color operator+(const Color &c) const { return Color(r+c.r, g+c.g, b+c.g, 1-(1-a)*(1-c.a)); }
	bool operator==(const Color &c) const { return (r==c.r && b==c.b && g==c.g && a==c.a); }
	bool operator!=(const Color &c) const { return !operator==(c); }
};

} // namespace GL
} // namespace Msp

#endif
