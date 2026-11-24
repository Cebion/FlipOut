/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/gl/texunit.h>
#include "balltype.h"
#include "resources.h"

using namespace std;
using namespace Msp;

BallType::BallType():
	radius(0.3),
	power(1),
	collect_bonuses(false),
	link_distance(0)
{ }


BallType::Loader::Loader(BallType &bt, Collection &c):
	Object::Loader(bt, c),
	btype(bt)
{
	add("collect_bonuses", &BallType::collect_bonuses);
	add("link_distance",   &BallType::link_distance);
	add("power",           &BallType::power);
	add("radius",          &BallType::radius);
}
