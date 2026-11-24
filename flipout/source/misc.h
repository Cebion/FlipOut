/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MISC_H_
#define MISC_H_

#include <ode/ode.h>

struct Vector3;

void get_matrix(dBodyID, double *);
void get_matrix(dGeomID, double *);
void draw_fullscreen_quad();
void normalize(Vector3 &);

#endif
