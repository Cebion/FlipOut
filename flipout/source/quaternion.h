/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef QUATERNION_H_
#define QUATERNION_H_

struct Vector3;

struct Quaternion
{
	double w, x, y, z;

	Quaternion(): w(1), x(0), y(0), z(0) { }
	Quaternion(double w_, double x_, double y_, double z_): w(w_), x(x_), y(y_), z(z_) { }
	static Quaternion from_axis_and_angle(const Vector3 &, double);
};

#endif
