/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef CAMERA_H_
#define CAMERA_H_

#include "vector3.h"

class FlipOut;

class Camera
{
private:
	float width;
	float height;
	float angle;
	float aspect;
	float left_margin;
	float right_margin;
	float dist;
	Vector3 pos;
	Vector3 dir;

public:
	Camera(float, float, float, float);
	void set_margins(float, float);
	void set_size(float, float);
	float get_angle() const { return angle; }
	const Vector3 &get_position() const { return pos; }
	const Vector3 &get_direction() const { return dir; }
	void apply() const;
	void unrotate() const;
	Vector3 get_ground_point(float, float);
private:
	void compute();
};

#endif
