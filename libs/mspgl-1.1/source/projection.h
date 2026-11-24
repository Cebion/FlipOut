/* $Id: projection.h 16 2007-09-22 17:01:48Z tdb $

This file is part of libmspgl
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_GL_PROJECTION_H_
#define MSP_GL_PROJECTION_H_

namespace Msp {
namespace GL {

/**
Sets up an orthogonal projection.
*/
void ortho(double, double, double, double, double, double);

/**
Sets up an orthogonal projection, with origin at the center of the screen.
*/
void ortho_centered(double, double);

/**
Sets up an orthogonal projection, with origin at the bottom left corner of the
screen.
*/
void ortho_bottomleft(double, double);

/**
Sets up an orthogonal projection, with origin at the top left corner of the
screen.  The Y coordinate is reversed and grows downwards.
*/
void ortho_topleft(double, double);

/**
Sets up a perspective projection.
*/
void frustum(double, double, double, double, double, double);

/**
Sets up a perspective projection, with origin at the center of the screen.
*/
void frustum_centered(double width, double height, double near, double far);

/**
Sets up a centered perspective projection in a simple way.
*/
void perspective(double fov_y, double aspect, double near, double far);

} // namespace GL
} // namespace Msp

#endif
