/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <ode/ode.h>
#include "quaternion.h"
#include "vector3.h"

Quaternion Quaternion::from_axis_and_angle(const Vector3 &axis, double angle)
{
	dQuaternion q;
	dQFromAxisAndAngle(q, axis.x, axis.y, axis.z, angle);
	return Quaternion(q[0], q[1], q[2], q[3]);
}
