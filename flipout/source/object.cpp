/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include "object.h"
#include "playfield.h"

using namespace Msp;

Object::Object(Playfield &pf):
	playfield(pf),
	active(false),
	stale(false)
{ }
