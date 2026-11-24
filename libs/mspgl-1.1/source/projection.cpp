/* $Id: projection.cpp 38 2007-12-29 20:47:08Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cmath>
#include "gl.h"
#include "projection.h"

namespace Msp {
namespace GL {

void ortho(double left, double right, double bottom, double top, double nearr, double farr)
{
	glOrtho(left, right, bottom, top, nearr, farr);
}

void ortho_centered(double width, double height)
{
	ortho(-width/2, width/2, -height/2, height/2, 0, 1);
}

void ortho_bottomleft(double width, double height)
{
	ortho(0, width, 0, height, 0, 1);
}

void ortho_topleft(double width, double height)
{
	ortho(0, width, height, 0, 0, 1);
}

void frustum(double left, double right, double bottom, double top, double nearr, double farr)
{
	glFrustum(left, right, bottom, top, nearr, farr);
}

void frustum_centered(double width, double height, double nearr, double farr)
{
	glFrustum(-width/2, width/2, -height/2, height/2, nearr, farr);
}

void perspective(double fov_y, double aspect, double nearr, double farr)
{
	double hh=tan(fov_y*M_PI/360)*nearr;
	frustum(-hh*aspect, hh*aspect, -hh, hh, nearr, farr);
}

} // namespace GL
} // namespace Msp
