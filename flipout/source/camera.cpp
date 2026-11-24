/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cmath>
#include <msp/gl/matrix.h>
#include <msp/gl/projection.h>
#include <msp/gl/transform.h>
#include "camera.h"
#include "flipout.h"

using namespace std;
using namespace Msp;

Camera::Camera(float w, float h, float a, float s):
	width(w),
	height(h),
	angle(a),
	aspect(s),
	left_margin(0),
	right_margin(0),
	dir(0, sin(angle), -cos(angle))
{
	compute();
}

void Camera::set_margins(float l, float r)
{
	left_margin=l;
	right_margin=r;
	compute();
}

void Camera::set_size(float w, float h)
{
	width=w;
	height=h;
	compute();
}

void Camera::apply() const
{
	GL::matrix_mode(GL::PROJECTION);
	GL::load_identity();
	float zoom=1-left_margin-right_margin;
	GL::scale(zoom, zoom, 1);
	GL::translate(left_margin-right_margin, 0, 0);
	float n=dist*0.2;
	GL::frustum_centered(n, n/aspect, n, n*10);

	GL::matrix_mode(GL::MODELVIEW);
	GL::load_identity();
	GL::rotate(angle*180/M_PI, -1, 0, 0);
	GL::translate(-pos.x, -pos.y, -pos.z);
}

void Camera::unrotate() const
{
	GL::rotate(angle*180/M_PI, 1, 0, 0);
}

Vector3 Camera::get_ground_point(float x, float y)
{
	Vector3 up(0, -dir.z, dir.y);
	Vector3 right(1, 0, 0);
	float zoom=1-left_margin-right_margin;
	Vector3 v=dir+up*(y/aspect/zoom*0.5)+right*((x/zoom-left_margin+right_margin)*0.5);
	return pos-v*(pos.z/v.z);
}

void Camera::compute()
{
	Vector3 front(0, -height*0.5, 2);
	Vector3 bottom(0, -height*0.5, 0);
	Vector3 top(0, height*0.5, 2);

	Vector3 up(0, -dir.z, dir.y);

	dist=width-dot(dir, front);

	float visible_aspect=aspect*(1-left_margin-right_margin);

	float offset;
	while(1)
	{
		float top_z=dist+dot(top, dir);
		float top_y=dot(top, up)/top_z;
		float bottom_z=dist+dot(bottom, dir);
		float bottom_y=dot(bottom, up)/bottom_z;

		float rel_h=(top_y-bottom_y)*visible_aspect;
		if(rel_h>1)
			dist*=rel_h;
		else
		{
			offset=(top_y+bottom_y)/(1/top_z+1/bottom_z);
			break;
		}
	}

	pos=Vector3(0, height*0.5, 0)-dir*dist+up*offset;
}
