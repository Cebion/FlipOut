/* $Id$

This file is part of flipout
Copyright © 2007  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/core/except.h>
#include "controls.h"

using namespace std;

Controls::Controls()
{
	move_left.pair(&move_right);
	move_left.set_dead_zone(0);
	move_left.set_threshold(0.8);
	move_right.set_dead_zone(0);
	move_right.set_threshold(0.8);

	flip_left.pair(&flip_right);

	by_name["move_left"]=&move_left;
	by_name["move_right"]=&move_right;
	by_name["flip_left"]=&flip_left;
	by_name["flip_right"]=&flip_right;
	by_name["turbo"]=&turbo;
	by_name["pause"]=&pause;
}

Msp::Input::Control &Controls::get(const string &name) const
{
	map<string, Msp::Input::Control *>::const_iterator i=by_name.find(name);
	if(i==by_name.end())
		throw Msp::KeyError("Unknown control", name);
	return *i->second;
}
