/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef POINT_H_
#define POINT_H_

struct Vector3
{
	double x, y, z;

	Vector3(): x(0), y(0), z(0) { }
	Vector3(double x_, double y_, double z_): x(x_), y(y_), z(z_) { }
	Vector3 operator+(const Vector3 &v) const { return Vector3(x+v.x, y+v.y, z+v.z); }
	Vector3 operator-(const Vector3 &v) const { return Vector3(x-v.x, y-v.y, z-v.z); }
	Vector3 operator-() const { return Vector3(-x, -y, -z); }
	Vector3 operator*(double f) const { return Vector3(x*f, y*f, z*f); }
	Vector3 operator/(double f) const { return Vector3(x/f, y/f, z/f); }
};

inline double dot(const Vector3 &v1, const Vector3 &v2) { return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; }

#endif
