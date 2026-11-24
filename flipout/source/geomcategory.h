/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef GEOMCATEGORY_H_
#define GEOMCATEGORY_H_

enum GeomCategory
{
	FLOOR=1,
	WALL=2,
	PADDLE=4,
	BALL=8,
	BRICK=16,
	BONUS=32,
	PROJECTILE=64
};

#endif
