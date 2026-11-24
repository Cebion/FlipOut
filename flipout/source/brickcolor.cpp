/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cstdlib>
#include "brickcolor.h"

using namespace Msp;

BrickColor::BrickColor():
	color(0.8),
	variation(0)
{ }

GL::Color BrickColor::random() const
{
	float f=rand()*2.0/RAND_MAX-1.0;
	return color+variation*f;
}


BrickColor::Loader::Loader(BrickColor &c):
	bcolor(c)
{
	add("color",     &Loader::color);
	add("variation", &Loader::variation);
}

void BrickColor::Loader::color(float r, float g, float b)
{
	bcolor.color=GL::Color(r, g, b);
}

void BrickColor::Loader::variation(float r, float g, float b)
{
	bcolor.variation=GL::Color(r, g, b);
}
