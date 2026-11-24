/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "paddletype.h"
#include "resources.h"

using namespace std;

PaddleType::PaddleType():
	width(4),
	height(0.5),
	depth(1),
	round_ends(false)
{ }


PaddleType::Loader::Loader(PaddleType &t, Collection &r):
	Object::Loader(t, r),
	ptype(t)
{
	add("size",       &Loader::size);
	add("round_ends", &PaddleType::round_ends);
}

void PaddleType::Loader::size(float w, float h, float d)
{
	ptype.width=w;
	ptype.height=h;
	ptype.depth=d;
}
